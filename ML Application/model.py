import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import LabelEncoder

# load data
data = pd.read_csv('water_data.csv')

# Drop the Time column
data.drop(data.columns[0], axis=1, inplace=True)

# separate target variable
target = data['target column']
features = data.drop('target column', axis=1)

# encode target variable
le = LabelEncoder()
target = le.fit_transform(target)

# build random forest classifier
rfc = RandomForestClassifier(n_estimators=100, max_depth=5, random_state=42)
rfc.fit(features, target)

# Make pickle file of our model
import pickle
pickle.dump(rfc, open("model.pkl", "wb"))