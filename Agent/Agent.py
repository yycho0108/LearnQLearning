import numpy as np

class Net:
    def __init__(self):
        pass
    def forward(self,s,a):# state-action
        pass
    def backProp(self,s,a,t):# state-action, target


class State:
    def __init__(self,prev=None,action=None):
        pass
    def reward(self):
        pass
    def getAction(self):
        pass
    def next(self,action=None):
        if action == None:
            action = self.getAction()
        return State(self,action)
    def isTerminal(self):
        pass

class Agent:
    def __init__(self):
        self.reset()
    def run_once(self):
        while not self.state.isTerminal():
            self.state = self.state.next()
        pass
    def run(self, num_iter=1):
        #run till terminal state
        for i in range(num_iter):
            self.run_once()
            self.reset()
            pass
    def reset(self):
        self.reward = 0
        self.state = self.getState()
    def getState(self):
        pass

def main():
    pass

if __name__ == "__main__":
    main()
