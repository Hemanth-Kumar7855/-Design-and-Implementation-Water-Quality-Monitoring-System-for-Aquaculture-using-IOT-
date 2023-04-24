from flask import Flask, request, jsonify, render_template
import numpy as np
import pickle

app = Flask(__name__)

# load the trained model
# model = pickle.load('model.pkl')
model = pickle.load(open("model.pkl", "rb"))

# define a function to preprocess the input data
def preprocess_input(data):
    # convert the input data to a numpy array
    data_array = np.array([data])

    # return the preprocessed data
    return data_array

# define a function to convert the output label to the actual class name
def convert_output(output):
    if output == 0:
        return 'Impure'
    elif output == 1:
        return 'Medium'
    elif output == 2:
        return 'Normal'

# define a route for the home page
@app.route('/')
def home():
    return render_template('index.html')

# define a route for the prediction endpoint
@app.route('/predict', methods=['POST'])
def predict():
    # get the input data from the request
    data = request.json

    # preprocess the input data
    data_array = preprocess_input(data)

    # make a prediction
    output = model.predict(data_array)[0]

    # convert the output label to the actual class name
    output_class = convert_output(output)

    # render the result template with the predicted output
    return render_template('result.html', prediction=output_class)

if __name__ == '__main__':
    app.run(debug=True)
