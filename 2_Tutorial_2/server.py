import random
import sys
from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route('/api/numbers', methods=['GET'])
def get_random_numbers():
    try:
        # Get query parameters with defaults
        num_count = int(request.args.get('count', 100))
        max_num = int(request.args.get('max', 1000))
        
        # Generate random unsorted numbers
        random_numbers = [random.randint(1, max_num) for _ in range(num_count)]
        
        # Return as JSON
        return jsonify({
            'success': True,
            'numbers': random_numbers,
            'count': len(random_numbers)
        })
    except Exception as e:
        return jsonify({
            'success': False,
            'error': str(e)
        }), 500

if __name__ == "__main__":
    # Parse command line arguments
    host = '0.0.0.0'
    port = 8080
    
    # Allow command line overrides
    if len(sys.argv) > 1:
        host = sys.argv[1]
    if len(sys.argv) > 2:
        port = int(sys.argv[2])
    
    print(f"Starting API server on {host}:{port}")
    print(f"API endpoint available at: http://{host}:{port}/api/numbers")
    print("Query parameters: count (default: 100), max (default: 1000)")
    print("Press Ctrl+C to stop the server")
    
    app.run(host=host, port=port)