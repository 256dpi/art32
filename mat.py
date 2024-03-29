import numpy as np

np.set_printoptions(precision=4, suppress=True)

print("--- test 2x2")
print(np.linalg.pinv(np.array([
    [0.1, 0.2],
    [0.3, 0.4],
])))

# [[-20.  10.]
#  [ 15.  -5.]]

print("--- test 2x3")
print(np.linalg.pinv(np.array([
    [0.1, 0.2, 0.1],
    [0.3, 0.4, 0.1],
])))

# [[-8.3333  5.    ]
#  [ 3.3333  0.    ]
#  [11.6667 -5.    ]]

print("--- test 3x3")
print(np.linalg.pinv(np.array([
    [0.1, 0.2, 0.1],
    [0.3, 0.4, 0.1],
    [0.1, 0.2, 0.3],
])))

# [[-25.  10.   5.]
#  [ 20.  -5.  -5.]
#  [ -5.   0.   5.]]

print("--- test 5x3")
print(np.linalg.pinv(np.array([
    [0.0, 0.1, 0.2, 0.0, 0.1],
    [0.0, 0.0, 0.0, 0.0, 0.0],
    [0.0, 0.3, 0.4, 0.0, 0.1],
])))

# [[ 0.      0.     -0.    ]
#  [-8.3333  0.      5.    ]
#  [ 3.3333  0.      0.    ]
#  [ 0.      0.      0.    ]
#  [11.6667  0.     -5.    ]]

print("--- test 3x5")
print(np.linalg.pinv(np.array([
    [0.0, 0.0, 0.0],
    [0.1, 0.0, 0.3],
    [0.2, 0.0, 0.4],
    [0.0, 0.0, 0.0],
    [0.1, 0.0, 0.1],
])))

# [[ 0.     -8.3333  3.3333  0.     11.6667]
#  [ 0.      0.      0.      0.      0.    ]
#  [-0.      5.      0.      0.     -5.    ]]

print("--- test 6x6")
print(np.linalg.pinv(np.array([
    [ +0.000, +0.000, +1.000, +0.000, +1.000, +0.000 ],
    [ +1.000, +0.000, +0.000, +1.000, +0.000, +0.000 ],
    [ +0.000, +1.000, +0.000, +0.000, +0.000, +1.000 ],
    [ -0.000, -60.000, -10.000, +0.000, +0.000, -43.000 ],
    [ +0.000, -20.000, +0.000, +0.00,0 +0.000, +30.000 ],
    [ +100.000, +0.000, +142.000, -95.000, -141.000, +0.000 ],
])))

# [[ 0.7231  0.4872  7.7208  0.1451 -0.0493  0.0051]
#  [-0.      0.      0.6    -0.     -0.02   -0.    ]
#  [ 0.     -0.     -5.32   -0.1     0.034   0.    ]
#  [-0.7231  0.5128 -7.7208 -0.1451  0.0493 -0.0051]
#  [ 1.      0.      5.32    0.1    -0.034   0.    ]
#  [-0.     -0.      0.4    -0.      0.02   -0.    ]]
