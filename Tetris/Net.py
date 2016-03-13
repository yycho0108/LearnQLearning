import numpy as np
from matplotlib import pyplot as plt

#Function generators
def XOR_GEN():
    I = np.random.rand(1,2)
    I = np.round(I).astype(int)
    O = I[0][0]^I[0][1]
    O = np.asarray(O)
    return I,O
def MULT_GEN():
    I = np.random.rand(1,2)
    O = I[0][0]*I[0][1]
    O = np.asarray(O)
    return I,O
def MOD_GEN():
    I = np.random.rand(1,2)
    O = np.mod(I[0][0],I[0][1])
    O = np.asarray(O)
    return I,O
def AVG_GEN():
    I = np.random.rand(1,2)
    O = (I[0][0]+I[0][1])/2
    O = np.asarray(O)
    return I,O
def SWAP_GEN():
    I = np.random.rand(1,2)
    O = (I[0][1],I[0][0])
    O = np.asarray(O)
    return I,O

def GEN():
    return XOR_GEN()


#TRANSFER FUNCTION
def sigmoid(x):
    return 1.0/(1.0 + np.exp(-x))
def sigmoidPrime(x):
    s = sigmoid(x)
    return s*(1.0-s)


#NETWORK
class Layer:
    def __init__(self,n):
        pass
class Net:
    def __init__(self,t):
        self.W = [] #weight matrices
        self.L = [] #Layers
        self.length = len(t)
        for l in t:
            self.L.append(Layer(l))
        for i in range(1,self.length):
            self.W.append(np.random.random((t[i-1],t[i])))
    def FF(self,X):
        #print(X)
        X = np.atleast_2d(X)
        self.L[0].O = X
        for i in range(1,self.length):
            self.L[i].I = np.dot(self.L[i-1].O,self.W[i-1])
            self.L[i].O = sigmoid(self.L[i].I)
        return self.L[-1].O
    def BP(self,X,Y):
        X = np.atleast_2d(X)
        Y = np.atleast_2d(Y)
        #print("X:",X)
        #print("Y:",Y)
        
        G = Y - self.FF(X) #ERROR Gradient : differentiated result of 0.5*(Y-X)^2
        self.L[-1].G = G * sigmoidPrime(self.L[-1].O)
        #calc Gradient
        for i in reversed(range(1,self.length-1)):
            self.L[i].G = np.dot(self.L[i+1].G,self.W[i].T) * sigmoidPrime(self.L[i].I)
        for i in range(1,self.length):
            dW = np.dot(self.L[i-1].O.T,self.L[i].G)
            self.W[i-1] += 0.3 * dW 
        return 0.5 * np.sum(G*G)

def main():
    t = [2,4,1]
    net = Net(t)
    G = []
    for i in range(9999):
        I,O = GEN()
        G.append(net.BP(I,O))
    I,O = GEN()

    print("INPUT : ", I)
    print("TARGET : ", O)
    print("OUT : ", net.FF(I))

    plt.plot(G)
    plt.show()


if __name__ == "__main__":
    main()
