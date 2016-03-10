import pygame,sys
from pygame.locals import *
import random
import numpy as np
import copy
#Simplified game of tetris where I can only drop from top without complex controls midair

CYAN = (0,255,255)
BLUE = (0,0,255)
ORANGE = (255,165,0)
YELLOW = (255,255,0)
GREEN = (0,255,0)
PURPLE = (255,0,255)
RED = (255,0,0)
BLACK = (0,0,0)
WHITE = (255,255,255)

BType = ["O","I","Z","S","L","J","T"]
BColor = [CYAN,BLUE,ORANGE,YELLOW,GREEN,PURPLE,RED]

B = [
[[0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0],[0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0],
 [0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0],[0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0]],
[[0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0],[0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0],
 [0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0],[0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0]],
[[0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0],[0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0],
 [0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0],[0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0]],
[[0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0],[0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0],
 [0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0],[0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0]],
[[0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0],[0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0],
 [0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0],[0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0]],
[[0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0],[0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0],
 [0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0],[0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0]],
[[0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0],[0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0],
 [0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0],[0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0]]
];

B = np.reshape(B,(7,4,4,4))

def findRange():
    br = []
    for t in range(7):#type
        br.append([])
        for r in range(4): #rotation
            br[t].append([])
            block = B[t][r]
            Z = []
            for col in range(4):
                Z += [np.count_nonzero(block[:,col])]

            i = 0
            while Z[i] == 0: #all zeros
                i += 1
            left = i

            i = 1
            while Z[-i] == 0:
                i += 1
            right = i-1
            br[t][r] = (left,right)
    return br
BRange = findRange()


class Block:
    def __init__(self):
        self.i = 0 #location
        self.j = 0

        self.r = random.randint(0,3) #rotation state
        self.t = random.randint(0,6) #block type

    def draw(self,screen):
        pass
        #Not really necessary, since it wouldn't be drawn anyways
        #t = self.t
        #r = self.r
        #for i in range(4):
        #    for j in range(4):
        #        if(B[t][r][i][j] == 1):
        #            pygame.draw.rect(screen,BColor[t],
    def __getitem__(self,i):
        t = self.t
        r = self.r
        return B[t][r][i]
    def valid(self):#w = width
        return BRange[self.t]

        #t = self.t
        #r = self.r
        #M = B[t][r]
        #Z = []
        #for i in range(4):
        #    Z += [np.count_nonzero(M[:,i])]
        #left = 0
        #right = w
        #
        #i = 0
        #while Z[i] == 0:
        #    i += 1
        #left -= i
        #i = 1
        #while Z[-i] == 0:
        #    i += 1
        #right += (i-1)
        #print(left,right)
        #return left,right
    def summary(self):
        return [(1.0 if i == self.t else 0.0) for i in range(7)]
        #return [self.t/7.0]

class Board:
    def __init__(self,w,h):
        self.w = w #width
        self.h = h #height
        self.board = np.zeros((h,w))
        self.over = False

    def draw(self,screen):
        screen.fill(WHITE)
        
        bw = screen.get_width()/self.w
        bh = screen.get_height()/self.h
        
        for i in range(self.h):
            for j in range(self.w):
                if self.board[i][j]:
                    pygame.draw.rect(screen,BLACK,(j*bw,i*bh,bw,bh))
    def check(self):
        row = []
        for i in range(self.h):
            if np.count_nonzero(self.board[i]) != self.w:# notfilled
                    row += [i]
        tmp = np.zeros((self.h-len(row),self.w))

        if len(row) != self.h: #there is a filled row
            self.board = np.insert(self.board[row],0,tmp,0)
        return self.h - len(row)

    def hit(self,block):
        for i in range(4):
            for j in range(4):
                if block[i][j] == 1 and (block.i+i >= self.h or self.board[block.i+i][block.j+j] == 1):
                    return True
        return False

    def __iadd__(self,block):
        while not self.hit(block):
            block.i += 1
        
        block.i -= 1
        if block.i < 0:
            self.over = True
        else:
            for i in range(4):
                for j in range(4):
                    if block[i][j] == 1:
                        self.board[block.i+i][block.j+j] = 1
        #self.check()
        return self
    def __add__(self,block):
        new = copy.deepcopy(self)
        new += block
        return new
    def getHighest(self,col):
        for i in range(self.h):
            if self.board[i][col] != 0:
                return self.h-i
        return 0
    def summary(self):
        return [self.getHighest(j) for j in range(self.w)]

class Tetris:
    def __init__(self,w,h):
        pygame.init()
        self.screen = pygame.display.set_mode((w*50,h*50))
        pygame.display.set_caption('Hello World!')
        
        self.w = w
        self.h = h

        self.board = Board(w,h)
        self.block = Block()
        self.nextBlock = Block()

    def draw(self):
        self.board.draw(self.screen)
        self.block.draw(self.screen)

    def run(self):
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                #elif event.type == KEYDOWN:
                #    key = pygame.key.get_pressed()
                #    
                #    if key[K_0]:
                #        self.block.j=0
                #    elif key[K_1]:
                #        self.block.j=1
                #    elif key[K_2]:
                #        self.block.j=2
                #    elif key[K_3]:
                #        self.block.j=3
                #    elif key[K_4]:
                #        self.block.j=4
                #    elif key[K_5]:
                #        self.block.j=5
                #    elif key[K_6]:
                #        self.block.j=6
                #    elif key[K_7]:
                #        self.block.j=7
                #    elif key[K_8]:
                #        self.block.j=8

                #    self.board += self.block
                #    if self.board.over:
                #        return
                #    self.block = self.nextBlock
                #    self.nextBlock = Block()
            pygame.time.wait(500)
            
            #Output To Decision Logic
            l,r = self.getAvailable()
            #self.block.t
            #self.board

            #Input From Decision Logic
            blockLoc = random.randint(l,r-1)

            self.put(blockLoc)
            if self.board.over:
                return
            self.draw()
            pygame.display.update()

    def getAvailable(self):
        return self.block.valid(self.w)

    def put(self,loc):
        self.block.j = loc;
        self.board += self.block
        self.block = self.nextBlock
        self.nextBlock = Block()

if __name__ == "__main__":
    tetris = Tetris(8,20)
    tetris.run()
