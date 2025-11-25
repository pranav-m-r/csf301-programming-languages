"""
HeteroList Web Application
A Flask-based web interface for the C++ HeteroList library.

This application provides a menu-driven interface to perform operations
on heterogeneous lists using the underlying C++ implementation.
"""

from flask import Flask, render_template, request, jsonify
import subprocess
import json
import os
import sys
import platform

app = Flask(__name__)

# Path to the C++ CLI executable (in CLIApp folder)
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CLI_DIR = os.path.join(SCRIPT_DIR, '..', 'CLIApp')

def get_wsl_path(windows_path):
    """Convert Windows path to WSL path."""
    # Replace drive letter with /mnt/drive_letter (lowercase)
    if len(windows_path) > 1 and windows_path[1] == ':':
        drive = windows_path[0].lower()
        rest = windows_path[2:].replace('\\', '/')
        return f"/mnt/{drive}{rest}"
    return windows_path.replace('\\', '/')

def call_cpp(command: str, *args) -> dict:
    """Call the C++ CLI with the given command and arguments."""
    try:
        if platform.system() == 'Windows':
            # On Windows, use WSL to run the Linux binary
            wsl_path = get_wsl_path(CLI_DIR)
            cli_cmd = f"cd {wsl_path} && ./hetero_cli {command}"
            for arg in args:
                # Escape the argument for shell
                escaped_arg = arg.replace('"', '\\"')
                cli_cmd += f' "{escaped_arg}"'
            
            result = subprocess.run(
                ['wsl', '-e', 'bash', '-c', cli_cmd],
                capture_output=True,
                text=True,
                timeout=10
            )
        else:
            # On Linux/Mac, run directly
            cli_path = os.path.join(CLI_DIR, 'hetero_cli')
            cmd = [cli_path, command] + list(args)
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=10
            )
        
        if result.returncode == 0 and result.stdout.strip():
            return json.loads(result.stdout.strip())
        else:
            return {"error": result.stderr or "Unknown error occurred"}
    except subprocess.TimeoutExpired:
        return {"error": "Operation timed out"}
    except json.JSONDecodeError as e:
        return {"error": f"Failed to parse response: {str(e)}"}
    except FileNotFoundError:
        return {"error": "C++ CLI not found. Please compile hetero_cli.cpp first."}
    except Exception as e:
        return {"error": str(e)}


@app.route('/')
def index():
    """Render the main page."""
    return render_template('index.html')


@app.route('/api/create', methods=['POST'])
def api_create():
    """Create a new list from input."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('create', list_data))


@app.route('/api/sort', methods=['POST'])
def api_sort():
    """Sort the list."""
    data = request.json
    list_data = data.get('list', '[]')
    order = data.get('order', 'asc')
    return jsonify(call_cpp('sort', list_data, order))


@app.route('/api/filter', methods=['POST'])
def api_filter():
    """Filter the list by type."""
    data = request.json
    list_data = data.get('list', '[]')
    filter_type = data.get('type', 'int')
    return jsonify(call_cpp('filter', list_data, filter_type))


@app.route('/api/map', methods=['POST'])
def api_map():
    """Apply a map operation to the list."""
    data = request.json
    list_data = data.get('list', '[]')
    operation = data.get('operation', 'double')
    return jsonify(call_cpp('map', list_data, operation))


@app.route('/api/reduce', methods=['POST'])
def api_reduce():
    """Apply a reduce operation to the list."""
    data = request.json
    list_data = data.get('list', '[]')
    operation = data.get('operation', 'sum')
    return jsonify(call_cpp('reduce', list_data, operation))


@app.route('/api/stats', methods=['POST'])
def api_stats():
    """Get statistics for the list."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('stats', list_data))


@app.route('/api/unique', methods=['POST'])
def api_unique():
    """Get unique elements from the list."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('unique', list_data))


@app.route('/api/reverse', methods=['POST'])
def api_reverse():
    """Reverse the list."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('reverse', list_data))


@app.route('/api/take', methods=['POST'])
def api_take():
    """Take first n elements."""
    data = request.json
    list_data = data.get('list', '[]')
    n = str(data.get('n', 1))
    return jsonify(call_cpp('take', list_data, n))


@app.route('/api/drop', methods=['POST'])
def api_drop():
    """Drop first n elements."""
    data = request.json
    list_data = data.get('list', '[]')
    n = str(data.get('n', 1))
    return jsonify(call_cpp('drop', list_data, n))


@app.route('/api/count', methods=['POST'])
def api_count():
    """Count elements by type."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('count', list_data))


@app.route('/api/inversions', methods=['POST'])
def api_inversions():
    """Count inversions in the list."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('inversions', list_data))


@app.route('/api/frequencies', methods=['POST'])
def api_frequencies():
    """Get frequency of each element."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('frequencies', list_data))


@app.route('/api/partition', methods=['POST'])
def api_partition():
    """Partition the list by predicate."""
    data = request.json
    list_data = data.get('list', '[]')
    partition_type = data.get('type', 'numeric')
    return jsonify(call_cpp('partition', list_data, partition_type))


@app.route('/api/push', methods=['POST'])
def api_push():
    """Push an element to the list."""
    data = request.json
    list_data = data.get('list', '[]')
    value = data.get('value', 'null')
    # Format value for CLI
    if isinstance(value, str) and not value.startswith('"') and value not in ['null', 'true', 'false']:
        try:
            float(value)
        except ValueError:
            value = f'"{value}"'
    return jsonify(call_cpp('push', list_data, str(value)))


@app.route('/api/pop', methods=['POST'])
def api_pop():
    """Pop an element from the list."""
    data = request.json
    list_data = data.get('list', '[]')
    return jsonify(call_cpp('pop', list_data))


if __name__ == '__main__':
    print("=" * 50)
    print("  HeteroList Web Application")
    print("=" * 50)
    print(f"\nScript Directory: {SCRIPT_DIR}")
    
    cli_path = os.path.join(SCRIPT_DIR, 'hetero_cli')
    if platform.system() != 'Windows' and not os.path.exists(cli_path):
        print("\n⚠️  Warning: hetero_cli executable not found!")
        print("   Please compile it first with:")
        print("   g++ -std=c++17 -o hetero_cli hetero_cli.cpp")
    elif platform.system() == 'Windows':
        print("\n📝 Note: Running on Windows - using WSL for C++ backend")
    
    print("\n🚀 Starting server at http://127.0.0.1:5000")
    print("   Press Ctrl+C to stop\n")
    
    app.run(debug=True, host='127.0.0.1', port=5000)
