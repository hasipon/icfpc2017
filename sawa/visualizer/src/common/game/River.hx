package game;
import game.SiteId;


class River
{
    public var id:RiverId;
    public var game:Game;
    public var a:SiteId;
    public var b:SiteId;
    public var owner:PunterId;
    
    public function new(game:Game, id:RiverId, a:SiteId, b:SiteId)
    {
        this.id = id;
        this.game = game;
        this.a = a;
        this.b = b;
        this.owner = PunterId.NotFound;
    }
    
    public function getAnother(source:SiteId):SiteId
    {
        return if (a == source) b else a;
    }
}
