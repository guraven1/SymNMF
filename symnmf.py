import sys
import numpy as np
import symnmf

# Reading user CMD arguments
if len(sys.argv) != 4:
    print("An Error Has Occurred")
    sys.exit(1)

k = int(sys.argv[1])
goal = sys.argv[2]
file_name = sys.argv[3]

# Check if goal is valid
valid_goals = ['symnmf', 'sym', 'ddg', 'norm']
if goal not in valid_goals:
    print("An Error Has Occurred")
    sys.exit(1)

# Helper function for H initialization when goal=symnmf
def initialize_H(W, k):
    np.random.seed(0)
    m = np.mean(W)
    H = np.random.uniform(0, 2 * np.sqrt(m/k), size=(W.shape[1], k))
    return H


# Call the appropriate C extension method based on the goal
if goal == 'symnmf':
    W = symnmf.norm(file_name)
    H = initialize_H(np.array(W), k)
    result = symnmf.updateH(H, W, k)
elif goal == 'sym':
    result = symnmf.sym(file_name)
elif goal == 'ddg':
    result = symnmf.ddg(file_name)
elif goal == 'norm':
    result = symnmf.norm(file_name)
else:
    print("An Error Has Occurred")
    sys.exit(1)

# Output the result matrix separated by a comma
for x in result:
    for i in range(len(result)):
        x[i] = str(format(x[i], ".4f"))
    print(",".join(x))

