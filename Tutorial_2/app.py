import os
import heapq
import tempfile
import threading
import requests
from flask import Flask, render_template, request, url_for, jsonify, send_file

# Configure Flask application
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = tempfile.gettempdir()
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # 16 MB upload limit
app.secret_key = 'sorting_application_key'

# Global variables for network results and status
network_results = [] 
network_status = "idle"
network_listener_thread = None

# ===================== SORTING CORE FUNCTIONALITY =====================

def sort_chunk(chunk):
    return sorted(chunk)

def merge_sorted_chunks(sorted_chunks):
    """Merge pre-sorted chunks using heapq.merge"""
    return heapq.merge(*sorted_chunks)

def sort_numbers(numbers=None, input_file=None, chunk_size=10000):
    if numbers is not None:
        # If data is already in memory, just sort and return
        return sort_chunk(numbers)
    
    elif input_file is not None:
        # For large files, process in chunks
        temp_files = []
        
        with open(input_file, 'r') as f:
            chunk = []
            
            for line in f:
                for num_str in line.strip().split():
                    try:
                        num = int(num_str)
                        chunk.append(num)
                        
                        # When chunk is full, sort and save to temp file
                        if len(chunk) >= chunk_size:
                            sorted_chunk = sort_chunk(chunk)
                            
                            # Write sorted chunk to temp file
                            fd, temp_path = tempfile.mkstemp()
                            temp_files.append(temp_path)
                            
                            with os.fdopen(fd, 'w') as tmp:
                                for n in sorted_chunk:
                                    tmp.write(f"{n}\n")
                            
                            chunk = []
                    except ValueError:
                        # Skip non-integer values
                        continue
            
            # Handle the last chunk if it exists
            if chunk:
                sorted_chunk = sort_chunk(chunk)
                
                fd, temp_path = tempfile.mkstemp()
                temp_files.append(temp_path)
                
                with os.fdopen(fd, 'w') as tmp:
                    for n in sorted_chunk:
                        tmp.write(f"{n}\n")
        
        # Open all temp files and merge them
        file_handles = []
        for temp_path in temp_files:
            file_handles.append(open(temp_path, 'r'))
        
        # Create iterators for each file
        iterators = [map(int, f) for f in file_handles]
        
        # Merge sorted iterators
        merged = list(merge_sorted_chunks(iterators))
        
        # Cleanup temp files
        for f in file_handles:
            f.close()
        for path in temp_files:
            os.remove(path)
        
        return merged
    
    else:
        raise ValueError("Either numbers or input_file must be provided")

# ===================== INPUT/OUTPUT HANDLERS =====================

def handle_keyboard_input(input_string):
    numbers = []
    for num_str in input_string.strip().split():
        try:
            numbers.append(int(num_str))
        except ValueError:
            continue
    return numbers

def handle_file_input(file_path, chunk_size=10000):
    # For small files, we can load everything into memory
    file_size = os.path.getsize(file_path)
    
    if file_size < chunk_size * 10:  # Arbitrary threshold
        numbers = []
        with open(file_path, 'r') as f:
            for line in f:
                for num_str in line.strip().split():
                    try:
                        numbers.append(int(num_str))
                    except ValueError:
                        continue
        return sort_numbers(numbers=numbers)
    else:
        # For large files, use the chunked approach
        return sort_numbers(input_file=file_path, chunk_size=chunk_size)

def handle_network_input(host='localhost', port=8080):
    global network_results, network_status
    
    try:
        network_status = "listening"
        print(f"Making API request to {host}:{port}")
        
        # Make HTTP request to the server API
        api_url = f"http://{host}:{port}/api/numbers"
        print(f"Requesting numbers from: {api_url}")
        
        response = requests.get(api_url, timeout=10)
        
        if response.status_code == 200:
            data = response.json()
            
            if data.get('success'):
                numbers = data.get('numbers', [])
                print(f"Received {len(numbers)} numbers from API")
                sorted_nums = sort_numbers(numbers=numbers)
                network_results = sorted_nums
                network_status = "received"
                print("API data received and sorted")
            else:
                network_status = "error"
                print(f"API error: {data.get('error', 'Unknown error')}")
        else:
            network_status = "error"
            print(f"HTTP error: {response.status_code}")
            
    except requests.exceptions.ConnectionError:
        network_status = "error"
        print(f"Connection error: Could not connect to {host}:{port}")
    except requests.exceptions.Timeout:
        network_status = "error"
        print("Request timed out")
    except Exception as e:
        network_status = "error"
        print(f"Error making API request: {e}")

# ===================== FLASK WEB APPLICATION =====================

@app.route('/')
def index():
    global network_status, network_results
    return render_template('index.html', 
                          network_status=network_status,
                          network_results=network_results if network_results is not None else [])

@app.route('/action', methods=['POST'])
def process_action():
    global network_status, network_results, network_listener_thread
    
    action_type = request.form.get('action_type')
    
    if action_type == 'keyboard':
        # Handle keyboard input
        input_text = request.form.get('input_text', '')
        numbers = handle_keyboard_input(input_text)
        sorted_numbers = sort_numbers(numbers=numbers)
        return jsonify({
            'success': True,
            'input_method': 'keyboard',
            'sorted_numbers': sorted_numbers,
            'input_data': input_text
        })
    
    elif action_type == 'file':
        # Handle file input
        if 'file' not in request.files:
            return jsonify({'success': False, 'error': 'No file part'})
        
        file = request.files['file']
        if file.filename == '':
            return jsonify({'success': False, 'error': 'No selected file'})
        
        # Save uploaded file
        input_path = os.path.join(app.config['UPLOAD_FOLDER'], 'input.txt')
        file.save(input_path)
        
        # Process file
        sorted_numbers = handle_file_input(input_path)
        
        return jsonify({
            'success': True,
            'input_method': 'file',
            'sorted_numbers': sorted_numbers,
            'file_name': file.filename
        })
    
    elif action_type == 'same_file':
        # Handle same file I/O
        if 'file' not in request.files:
            return jsonify({'success': False, 'error': 'No file part'})
        
        file = request.files['file']
        if file.filename == '':
            return jsonify({'success': False, 'error': 'No selected file'})
        
        # Save uploaded file
        input_path = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
        file.save(input_path)
        
        # Process file
        sorted_data = handle_file_input(input_path)
        
        # Write back to same file
        with open(input_path, 'w') as f:
            for num in sorted_data:
                f.write(f"{num}\n")
        
        return jsonify({
            'success': True,
            'input_method': 'same_file',
            'file_name': file.filename,
            'download_url': url_for('download_file', filename=file.filename)
        })
    
    elif action_type == 'network':
        # Start network listener
        host = request.form.get('host', 'localhost')
        port = int(request.form.get('port', 8080))
        
        # Reset previous state
        network_status = "starting"
        network_results = []  # Initialize as empty list, not None
        
        # Kill any existing thread
        if network_listener_thread and network_listener_thread.is_alive():
            pass
        
        # Start new thread
        network_listener_thread = threading.Thread(target=handle_network_input, 
                                                  args=(host, port))
        network_listener_thread.daemon = True
        network_listener_thread.start()
        
        return jsonify({
            'success': True,
            'network_status': network_status,
            'host': host,
            'port': port
        })
    
    return jsonify({'success': False, 'error': 'Unknown action type'})

@app.route('/download/<filename>')
def download_file(filename):
    return send_file(os.path.join(app.config['UPLOAD_FOLDER'], filename), 
                    as_attachment=True, download_name=filename)

@app.route('/check_network_status')
def check_network_status():
    global network_status, network_results
    
    # For "received" status, include the results in the same format as other methods
    if network_status == "received":
        return jsonify({
            'status': network_status,
            'success': True,
            'input_method': 'network',
            'sorted_numbers': network_results,
            'host_info': f"API server"
        })
    else:
        return jsonify({
            'status': network_status,
            'results': []
        })

# Main function
def main():
    # Start Flask app
    app.run(debug=True)

if __name__ == '__main__':
    main()