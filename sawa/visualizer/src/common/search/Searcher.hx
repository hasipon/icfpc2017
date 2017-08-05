package search;
import game.Game;
import game.PunterId;
import game.command.MoveStruct;

interface Searcher {
    public function getMove(game:Game, punter:PunterId):MoveStruct;
}