package core;
import game.Game;
import game.PunterId;
import game.command.MoveStruct;
import game.command.ScoreStruct;
import game.command.StopStruct;

class PlayingState 
{
    public var parent:RootContext;
    public var moves:Array<MoveStruct>;
    public var scores:Array<ScoreStruct>;
    public var you:PunterId;
    public var currentIndex = 0;
    public var playing:Bool;
    
    public function new(
        parent:RootContext, 
        you:PunterId,
        moves:Array<MoveStruct>,
        scores:Array<ScoreStruct>
    ) {
        this.parent = parent;
        this.you = you;
        this.moves = moves;
        this.scores = scores;
        playing = true;
    }
    
    public function update():Void
    {
        trace(playing);
        if (playing)
        {
            doMove();
        }
    }
    
    public function togglePlaying()
    {
        playing = !playing;
        parent.updateUi();
    }
    
    public function doMove()
    {
        if (moves.length <= currentIndex)
        {
            playing = false;
            parent.updateUi();
            return;
        }
        
        parent.game.addMove(moves[currentIndex]);
        currentIndex += 1;
        parent.updatePixi();
    }
    
    public function undoMove():Void
    {
        if (currentIndex > 0)
        {
            parent.game.undoMove();
            currentIndex -= 1;
            parent.updatePixi();
        }
    }

}
