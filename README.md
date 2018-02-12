# Microprocessors and Digital Systems


## Scenario
The Project is a fully functional Pong clone written in ncurses. The player controls a game paddle positioned near one edge of the terminal window which moves vertically but not horizontally. A ball flies within the playing area of the game, bouncing off floor and ceiling and one of the walls, but passing through the remaining wall. The game has four levels which entail qualitatively distinct modes of play.

## Built With
 * [C Programming Language]
 * The Pong game requires the ZDK library.
 
```
cd ZDK
make
gcc -o a1_n8779333 *.c -std=gnu99 -L ../ZDK -I ../ZDK -lzdk -lncurses -lm
./a1_n8779333
```

## Author

* **Sara Alraddadi** - *Initial work* - [Sara-Alraddadi](https://github.com/Sara-Alraddadi)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
ZDK is owned by their respective authors.


## Acknowledgments

* For more information about the project please refer to (Pong-Game-Project/CAB202_PongGame_Info.pdf).
