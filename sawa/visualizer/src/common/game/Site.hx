package game;

class Site
{
    public var game:Game;
    public var id:SiteId;
    public var isMine:Bool;
    public var rivers:Map<SiteId, River>;
    public var x:Float;
    public var y:Float;
    
    public function new (game:Game, id:SiteId, x:Float, y:Float)
    {
        this.x = x;
        this.y = y;
        this.id = id;
        this.game = game;
        isMine = false;
        rivers = new Map();
    }
}
