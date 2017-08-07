package game;
import game.SiteId;
import haxe.ds.Option;


class River
{
    public var id:RiverId;
    public var game:Game;
    public var a:SiteId;
    public var b:SiteId;
    public var owner:PunterId;
    public var option:Option<PunterId>;
    
    public function new(game:Game, id:RiverId, a:SiteId, b:SiteId)
    {
        this.id = id;
        this.game = game;
        this.a = a;
        this.b = b;
        this.owner = PunterId.NotFound;
        this.option = Option.None;
    }
    
    public inline function getAnother(source:SiteId):SiteId
    {
        return if (a == source) b else a;
    }
}
