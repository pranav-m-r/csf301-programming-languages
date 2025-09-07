import os
import heapq
import tempfile
import requests
from flask import Flask, render_template, request, url_for, redirect, flash, send_file
from werkzeug.utils import secure_filename

# Configure Flask application
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = tempfile.gettempdir()
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # 16MB max upload size
app.secret_key = 'sorting_application_key'

# ==================== SORTING FUNCTIONALITY ====================

def detect_data_type(value_str):
    """Detect if a string represents an int, float, or leave as string"""
    # Try integer first
    try:
        return int(value_str)
    except ValueError:
        pass
    
    # Try float
    try:
        return float(value_str)
    except ValueError:
        # Return as string if not numeric
        return value_str.strip()

def parse_input(input_data, chunk_size=10000):
    """Parse input into chunks of values with appropriate types"""
    chunks = []
    current_chunk = []
    
    # If input_data is a string (keyboard/text input)
    if isinstance(input_data, str) and os.path.isfile(input_data):
        with open(input_data, 'r') as f:
            for line in f:
                for value_str in line.split():
                    if value_str:
                        current_chunk.append(detect_data_type(value_str))
                        
                        # Create a new chunk if the current one is full
                        if len(current_chunk) >= chunk_size:
                            chunks.append(current_chunk)
                            current_chunk = []
    
    # If input_data is a file path
    elif isinstance(input_data, str):
        # Split by whitespace and convert to appropriate types
        for value_str in input_data.split():
            if value_str:
                current_chunk.append(detect_data_type(value_str))
                
                # Create a new chunk if the current one is full
                if len(current_chunk) >= chunk_size:
                    chunks.append(current_chunk)
                    current_chunk = []
    
    # Append the last chunk if not empty
    if current_chunk:
        chunks.append(current_chunk)
    
    return chunks

def sort_chunks(chunks):
    """Sort all chunks"""
    return [sorted(chunk) for chunk in chunks]

def merge_sorted_chunks(sorted_chunks):
    """Merge all sorted chunks into one sorted list"""
    # For small total data, just concatenate and sort again
    if sum(len(chunk) for chunk in sorted_chunks) < 100000:
        all_items = []
        for chunk in sorted_chunks:
            all_items.extend(chunk)
        return sorted(all_items)
    
    # For large data, use heapq.merge
    return list(heapq.merge(*sorted_chunks))

def sort_data(input_data, chunk_size=10000):
    """Main sorting function that handles all input types"""
    # Parse input into chunks
    chunks = parse_input(input_data, chunk_size)
    
    # Check if we have mixed types across all chunks
    all_items = [item for chunk in chunks for item in chunk]
    has_string = any(isinstance(item, str) and not isinstance(item, (int, float)) for item in all_items)
    has_number = any(isinstance(item, (int, float)) for item in all_items)
    
    # If we have mixed types, convert everything to string
    if has_string and has_number:
        string_chunks = [[str(item) for item in chunk] for chunk in chunks]
        sorted_chunks = [sorted(chunk) for chunk in string_chunks]
    else:
        # Sort each chunk with native types
        sorted_chunks = [sorted(chunk) for chunk in chunks]
    
    # Merge sorted chunks
    return merge_sorted_chunks(sorted_chunks)

# ==================== ROUTE HANDLERS ====================

@app.route('/')
def index():
    """Render main page"""
    return render_template('index.html')

@app.route('/sort', methods=['POST'])
def sort_handler():
    """Handle all sorting operations"""
    input_method = request.form.get('input_method', '')
    
    try:
        # === KEYBOARD INPUT ===
        if input_method == 'keyboard':
            input_text = request.form.get('input_text', '')
            if not input_text.strip():
                flash('Please enter some data to sort', 'warning')
                return redirect('/')
            
            sorted_data = sort_data(input_text)
            return render_template('index.html', 
                                   active_tab='keyboard',
                                   sorted_data=sorted_data,
                                   result_source='Keyboard Input',
                                   item_count=len(sorted_data))
        
        # === FILE INPUT ===
        elif input_method == 'file':
            if 'file' not in request.files:
                flash('No file part', 'error')
                return redirect('/')
                
            file = request.files['file']
            if file.filename == '':
                flash('No selected file', 'error')
                return redirect('/')
            
            # Save uploaded file
            filename = secure_filename(file.filename)
            file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(file_path)
            
            # Sort file data
            sorted_data = sort_data(file_path)
            
            return render_template('index.html', 
                                   active_tab='file',
                                   sorted_data=sorted_data,
                                   result_source=f'File: {filename}',
                                   item_count=len(sorted_data))
        
        # === SAME FILE I/O ===
        elif input_method == 'samefile':
            if 'file' not in request.files:
                flash('No file part', 'error')
                return redirect('/')
                
            file = request.files['file']
            if file.filename == '':
                flash('No selected file', 'error')
                return redirect('/')
            
            # Save uploaded file
            filename = secure_filename(file.filename)
            file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(file_path)
            
            # Sort file data
            sorted_data = sort_data(file_path)
            
            # Write sorted data back to file
            with open(file_path, 'w') as f:
                for item in sorted_data:
                    f.write(f"{item}\n")
            
            return render_template('index.html', 
                                   active_tab='samefile',
                                   download_filename=filename,
                                   result_source=f'Saved to file: {filename}',
                                   item_count=len(sorted_data))
        
        # === NETWORK INPUT ===
        elif input_method == 'network':
            host = request.form.get('host', 'localhost')
            port = request.form.get('port', '8080')
            
            # Make request to server API
            api_url = f"http://{host}:{port}/api/numbers"
            try:
                response = requests.get(api_url, timeout=10)
                
                if response.status_code == 200:
                    data = response.json()
                    
                    if data.get('success'):
                        numbers = data.get('numbers', [])
                        sorted_data = sort_data(' '.join(map(str, numbers)))
                        
                        return render_template('index.html', 
                                              active_tab='network',
                                              sorted_data=sorted_data,
                                              result_source=f'Network API ({host}:{port})',
                                              item_count=len(sorted_data))
                    else:
                        flash(f'API error: {data.get("error", "Unknown error")}', 'error')
                        return render_template('index.html', active_tab='network')
                else:
                    flash(f'HTTP error: {response.status_code}', 'error')
                    return render_template('index.html', active_tab='network')
                    
            except requests.exceptions.ConnectionError:
                flash(f'Connection error: Could not connect to {host}:{port}', 'error')
                return render_template('index.html', active_tab='network')
            except requests.exceptions.Timeout:
                flash('Request timed out', 'error')
                return render_template('index.html', active_tab='network')
            except Exception as e:
                flash(f'Error: {str(e)}', 'error')
                return render_template('index.html', active_tab='network')
        
        else:
            flash('Invalid input method', 'error')
            return redirect('/')
            
    except Exception as e:
        flash(f'Error: {str(e)}', 'error')
        return redirect('/')

@app.route('/download/<filename>')
def download_file(filename):
    """Download a sorted file"""
    return send_file(os.path.join(app.config['UPLOAD_FOLDER'], filename), 
                    as_attachment=True, download_name=filename)

if __name__ == '__main__':
    app.run(debug=True)