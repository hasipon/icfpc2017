package game;


class River
{
    public var game:Game;
    public var a:SiteId;
    public var b:SiteId;
    
    public function new(game:Game, a:Site, b:Site)
    {
        this.game = game;
        this.a = a;
        this.b = b;
    }
}
