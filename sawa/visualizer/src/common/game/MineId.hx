package game;

abstract MineId(SiteId) to SiteId
{
    public function new(value:SiteId)
    {
        this = value;
    }
}

