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
    public var punterNames:Array<String>;
    public var you:PunterId;
    public var currentIndex = 0;
    public var playing:Bool;
    public var rest:Float;
    
    public function new(
        parent:RootContext, 
        you:PunterId,
        moves:Array<MoveStruct>,
        scores:Array<ScoreStruct>,
        punterNames:Array<String>
    ) {
        this.parent = parent;
        this.you = you;
        this.moves = moves;
        this.scores = scores;
        this.punterNames = punterNames;
        rest = 0;
        playing = true;
    }
    
    public function update(frame:Float):Void
    {
        if (playing)
        {
            rest += frame;
            while (rest >= 1)
            {
                rest -= 1;
                doMove();
                if (!playing)
                {
                    rest = 0;
                    break;
                }
            }
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
        parent.updateUi();
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
    
    
    public function gotoTop() 
    {
        while (currentIndex > 0)
        {
            parent.game.undoMove();
            currentIndex -= 1;
        }
        
        parent.updatePixi();
    }
    
    public function gotoEnd() 
    {
        while (true)
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
    }
    
    public function changeIndex(index:Int):Void
    {
        currentIndex = index;
        parent.updateUi();
    }
}
