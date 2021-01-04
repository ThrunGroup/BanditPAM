import timeit

import numpy as np
import pandas as pd
import scipy.stats as st
import yep
from BanditPAM import KMedoids

times = []

def runtime_and_profile(dataset_size, fixed_perm = True, k=5):
    print("running mnist-{}k-parallelized-cache-k-{}".format(dataset_size, k))
    X = pd.read_csv('data/MNIST-{}k.csv'.format(dataset_size), sep=' ', header=None).to_numpy()

    # Fit the data with BanditPAM:
    start = timeit.default_timer()
    if not fixed_perm:
        test = False
        name = "mnist-{}k-parallelized-cache-k-{}-{}".format(dataset_size, k, "no-fixed-perm")
        print("no fixed perm")
    else:
        test = True
        name = "mnist-{}k-parallelized-cache-k-{}-{}".format(dataset_size, k, "fixed-perm")
        print("fixed perm")
    kmed = KMedoids(n_medoids = k, algorithm = "BanditPAM", verbosity=1, logFilename=name,cache = True, use_fixed_perm = test)
    kmed.fit(X, 'L2', k, name)
    elapsed = start - timeit.default_timer()
    times.append(-elapsed)
    print(elapsed)
    print(kmed.final_medoids)
for val in [20]:
    runtime_and_profile(val, True)

# sizes = [1, 10, 20, 70]
# pool = mp.Pool(processes=len(sizes))
# pool.map(runtime_and_profile, sizes)
