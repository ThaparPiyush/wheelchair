import os
import pickle

if not os.path.isfile("encodings.pickle"):
    with open("encodings.pickle",'wb') as file:
        pickle.dump("some obejct", file)
