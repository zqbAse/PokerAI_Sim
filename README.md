# Poker Agent using MCCFR

* Trains strategies for a simple Poker
* Supported Training methods:
  * External Sampling
  * Chance Sampling
* The simple Poke rule:
  * the game contain 6 cards(1,2,3,4,5,6)
  * the game take action rule is the same as Holdem.
  * The game is divided into three rounds:
    * In every round, one 'fold' lead to the game ending, and all players take 'call' action, which is simular to Holdem.
    * first round: every player have one hole card and they interactively take action.
    * second round: deal two community cards and they interactively take action by Holdem rule.
    * third round : deal one community card. If all player call. The player who have highest hand strength win all pot.
    * hand strength: every player combine one his one hole card with three community cards as the total of four cards.Hand strength is the max hand strength of three cards selected from the four cards.
    (456 > 345 > 234 > 123 > 6 > 5 > 4 > 3 > 2 > 1) 

## Requirements
* For C++11 support
* GraphViz software

## Installation
* Clone repositories:
```
$ git clone https://github.com/zqbAse/PokerAI_Sim.git
```

* Compile and Run:
```shell
$ cd zqbAse/PokerAI_Sim
$ g++ blueprint.cpp -o blueprint.o -std=c++11
$ ./blueprint.o
```


## Usage
If the build process was successful 4 binaries have been created:

* ./Poker is the foundation class of the poker game.
* ./Util have one class of generateing random number.
* ./Tree contain data structure of tree node, bulid game tree, compute game tree exploitability, save and load game tree policy, and visualize game tree code.
*  blueprin.cpp is main algorithm of mccfr.

## Poker folder

the foundation class of the poker game. 
##### Card.h
* every card class, it's id range from 0 to 5, which map figure 1 to 6.
##### Deck.h
* deck class of cards, it contains 6 cards.
##### Engine.h
* it compute game result, judging win person and the person can get the number of chips.
##### Player.h
* player class,it's attributes contain initial chips, bet chips, small or big blind. 
it's functions contain raise, call and fold.
##### Table.h
* it's attributes contain players, pot and community cards.
##### State.h
it is game state, contain every players infoset.  
* constructor function contian deal every player hole card, three community cards and have mapped (hole card,community cards) to a branch in chance node.
* function **reset_game()** is reset game to initial game.  
* function **move_to_next_player()** is that after a player takes action, turning to next player who can take action.  
* function **reset_betting_round_state()** is to control raise chips and check situation.  
* function **is_terminal()** is to judge game over, including one player remained or river round over.  
* function **payout(int i)** is to compute player i winning or losing chips.  
* function **apply_action(char actionstr)** is that when a player take action, game state is change.  
* function **increment_stage()** is that when every game round over, turning to next round.  
* function **legal_actions()** is to return legal actions in current state. 

### Tree folder
##### Node.h
* data structure of every node in game tree.
##### Bulid_Tree.h
* traverse every possible hole card, community cards and legal actions to bulid the game tree.
##### Exploitability.h
* it compute the exploitability of game tree policy.
##### Save_load.h
* save or load the game tree policy. 
it's functions contain raise, call and fold.
##### Visualize_Tree.h
* Visualize game Tree.After the game run out, current folder will generate a 'blueprint_sim.stgy' file.
```shell
$ cd GraphViz/bin
$ dot -Tpng blueprint_sim.stgy > temp.png
```
* temp.png is the game tree.
### Part of Tree for player 0 exploiting player 1
![百度](img/partoftree.png "百度图片")
* red node is new round node by dealing card from chance node. 
* black and red node is player 0 who need to take action.
* blue is player 1 who need to take action.
* the value in the node is best response value for player 1.
* red node '1243' is current player hole card '1',flop community cards'24',turn community cards'3'.
* 'd':fold,'l':call,'n':allin.
* the number of edge is the possibilitiy of the action. 100 is 100%, 50 is 50%.
