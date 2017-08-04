package game;


class Punter
{
    public var game:Game;
    public var id:PunterId;
    public var score:Int;
    public var rivers:Set<RiverId>;
    public var accessableSites:Map<MineId, Set<SiteId>>
    
    public function new (
        game:Game, 
        id:PunterId
    )
    {
        this.game = game;
        this.id = id;
        score = 0;
        rivers = new Set();
        accessableSites = new Map<MineId, Set<SiteId>>();
    }
}
