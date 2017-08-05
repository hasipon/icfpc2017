package game;


class Punter
{
    public var game:Game;
    public var id:PunterId;
    public var accessableSites:Map<SiteId /* for Mine */, Set<SiteId>>;
    
    public function new (
        game:Game, 
        id:PunterId
    )
    {
        this.game = game;
        this.id = id;
        accessableSites = new Map<SiteId, Set<SiteId>>();
    }
}
