#!/usr/bin/python
import random
import pygame
from pygame.locals import *

from Tetris import *
from Net import *
import numpy as np

import pickle

from matplotlib import pyplot as plt

ALPHA = 0.4
GAMMA = 0.3

W = 8
H = 20

class State:
    def __init__(self):
        pass

class Action:
    def __init__(self):
        pass

class Agent:
    def __init__(self):
        pass

scaleFactor = 1/50.0

class TetrisState(State):
    def __init__(self,prev=None,shape=None): #board,block
        if prev == None:
            self.board = Board(*shape)
            self.block = Block()
            self.reward = 0.0
        else:
            self.board = prev.board + prev.block
            self.block = prev.nextBlock
            self.reward = (.51*self.spaceEmpty() + .76*self.lineClear()) * scaleFactor #prev.reward + self.getReward()
        self.nextBlock = Block()

    def getActions(self):
        return self.block.valid()
    def next(self,a):
        self.block.r = a.rot
        self.block.j = a.loc
        return TetrisState(self)
    def spaceEmpty(self): #first-pass reward model with Immediate Reward
            return (self.board.h - max(self.board.summary())) # lower height = better.
    def summary(self):
        #board
        s = self.board.summary()
        m = min(s)
        s = [float(h-m)/self.board.h for h in s]
        return s + self.nextBlock.summary() + self.block.summary()#block summary is just type.
    def lineClear(self):
        if self.board.over:
            return -1
        else:
            c = self.board.check()
            #if c != 0:
            #    print ("########JACKPOT!!#########")
            return 1 + 5*c 
    def done(self):
        return self.board.over
    def draw(self,screen):
        self.board.draw(screen)

class TetrisAction(Action):
    def __init__(self,rot,loc):
        self.rot = rot
        self.loc = loc
    def summary(self):
        return [(1.0 if self.rot==i else 0.0) for i in range(4)] + [(1.0 if self.loc==i else 0.0) for i in range(-3,W+3)]
        #return [self.rot/3.0,self.loc/float(W)]
    def __repr__(self):
        return self.__str__()
    def __str__(self):
        return "R:{},L:{}".format(self.rot,self.loc)


class TetrisAgent(Agent):
    def __init__(self,shape):
        self.shape = shape
        self.w = shape[0]
        self.h = shape[1]
        
        W = self.w
        H = self.h

        self.state = None
        #1 for block, 1 for nextblock, 2 for action(loc/rot of block)
        #8 for each height in board width, outputing 1 Q-value
        I = len(TetrisState(shape=shape).summary() + TetrisAction(0,0).summary())
        t = [I,I/2,I/2,1]
        self.net = Net(t)
        print(self.net.W)
    def chooseBest(self,s):
        actions = self.state.getActions()
        a = None 
        maxQ = -50000 

        #qList = []
        #aList = []
        for r,lr in enumerate(actions): #rotation
            #print(lr)
            for l in range(0-lr[0],self.w+lr[1]-3): #location(j)
                _a = TetrisAction(r,l)
                q = self.net.FF(s+_a.summary())
                #qList += [q]
                #print(q)
                if q > maxQ:
                    maxQ = q
                    a = _a
        #print('A:', a)
        #print('Q:',maxQ)
        return a,maxQ

    def chooseRand(self,s):
        actions = self.state.getActions()
        r = np.random.randint(4)
        lr = actions[r]
        l = np.random.randint(0-lr[0],self.w + lr[1] - 3)
        a = TetrisAction(r,l)
        return a, self.net.FF(s+a.summary()) 

    def chooseNext(self,s): #choose "best next state"
        if np.random.random()<0.1:
            return self.chooseRand(s)
        else:
            return self.chooseBest(s)

    def draw(self,screen):
        self.state.draw(screen)

    def run(self,screen=None,delay=0):
        self.state = TetrisState(shape=self.shape)
        epoch = 0
        while not self.state.done():
            epoch += 1
            for event in pygame.event.get():
                if event.type == QUIT:
                    return -1
            s = self.state.summary()
            a,u = self.chooseNext(s) #select action
            #print(a.summary())
            self.state = self.state.next(a)
            _,_u = self.chooseBest(s) #best of next
            u2 = (1-ALPHA)*u + ALPHA*(self.state.reward + GAMMA*_u)
            #if self.state.getReward() == -100:
            #    print("U:{}, U2:{}".format(u, u2))
            self.net.BP(s+a.summary(),u2) #update Q value
            if screen is not None:
                self.draw(screen)
                pygame.display.update()
                if delay != 0:
                    pygame.time.wait(delay)
            #raw_input("...")
        return epoch 

if __name__ == "__main__":
    w,h = 10,20
    pygame.init()
    screen = pygame.display.set_mode((w*50,h*50))
    pygame.display.set_caption('Tetris_AI')
    agent = TetrisAgent((w,h))
    #with open('agent','r') as f:
    #    agent = pickle.load(f)

    scores = []
    for i in range(10000):
        score = agent.run()
        if score == -1:
            break
        scores += [score]
        print("[{}] SCORE : {}".format(i,score))
        #raw_input("...")
    raw_input("...")
    agent.run(screen,500)
    with open('agent','w') as f:
        pickle.dump(agent,f)

    plt.plot(scores)
    plt.show()

