package game;


class River
{
    public var id:RiverId;
    public var game:Game;
    public var a:SiteId;
    public var b:SiteId;
    
    public function new(game:Game, id:RiverId, a:SiteId, b:SiteId)
    {
        this.id = id;
        this.game = game;
        this.a = a;
        this.b = b;
    }
}
