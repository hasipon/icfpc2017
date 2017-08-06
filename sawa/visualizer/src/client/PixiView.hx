package;
import core.PlayingState;
import core.RootContext;
import game.Punter;
import game.PunterId;
import haxe.ds.Option;
import js.Browser;
import pixi.core.graphics.Graphics;
import pixi.core.sprites.Sprite;
import pixi.core.textures.Texture;
import pixi.plugins.app.Application;
import tweenxcore.Tools.FloatTools;
import tweenxcore.color.HsvColor;

class PixiView extends Application 
{
    static var WIDTH = 900;
    static var HEIGHT = 650;
    
    var _graphic:Graphics;
    var rootContext:RootContext;
    
    
	public function new(rootContext:RootContext) {
		super();
        this.rootContext = rootContext;

		position = Application.POSITION_FIXED;
		width = WIDTH;
		height = HEIGHT;
        autoResize = false;
        this.position = "relative";
		backgroundColor = 0x006666;
		transparent = true;
		antialias = false;
		super.start("auto", Browser.document.getElementById("pixi"));

		_graphic = new Graphics();
        drawBackground();
        
		stage.addChild(_graphic);
	}
    
    function drawBackground() {
        _graphic.clear();
		_graphic.beginFill(0xF9F9F9, 1);
		_graphic.drawRect(0, 0, WIDTH, HEIGHT);
		_graphic.endFill();
    }
        
    public function update():Void
    {
        drawBackground();
        
        var game = rootContext.game;
        
        var top    = Math.POSITIVE_INFINITY;
        var bottom = Math.NEGATIVE_INFINITY; 
        var left   = Math.POSITIVE_INFINITY;
        var right  = Math.NEGATIVE_INFINITY;
        
        for (site in game.sites)
        {
            if (top    > site.y) top    = site.y;
            if (bottom < site.y) bottom = site.y;
            if (left   > site.x) left   = site.x;
            if (right  < site.x) right  = site.x;
        }
        
        inline function resolveX(x:Float):Float
        {
            return lerp(inverseLerp(x, left, right), 20, WIDTH - 20) + Math.random() * 5;
        }
        
        inline function resolveY(y:Float):Float
        {
            return lerp(inverseLerp(y, top, bottom), 20, HEIGHT - 20) + Math.random() * 5;
        }
        
        var you = switch (rootContext.playingState)
        {
            case Option.None:
                PunterId.NotFound;
                
            case Option.Some(_playingState):
                _playingState.you;
        }
        
        for (river in game.rivers)
        {
            var ax = resolveX(game.sites[river.a].x);
            var ay = resolveY(game.sites[river.a].y);
            var bx = resolveX(game.sites[river.b].x);
            var by = resolveY(game.sites[river.b].y);
            
            _graphic.lineStyle(1, getColor(river.owner, you).toRgbInt(), 0.7);
            
            _graphic.moveTo(ax, ay);
            _graphic.lineTo(bx, by);
        }
        
        _graphic.lineStyle(1, 0x00CC22, 0.8);
        for (site in game.sites)
        {
            var x = resolveX(site.x);
            var y = resolveY(site.y);
            
            _graphic.drawCircle(x, y, 5);
        }
        
        _graphic.lineStyle(1, 0xCC0022, 0.8);
        for (site in game.mines)
        {
            var x = resolveX(site.x);
            var y = resolveY(site.y);
            
            _graphic.drawCircle(x, y, 10);
        }
        
    }
    
    public static function getColor(punter:PunterId, you:PunterId):HsvColor
    {
        return if (punter == PunterId.NotFound)
        {
            new HsvColor(0, 0, 0.92);
        }
        else if (punter == you)
        {
            new HsvColor(0, 1, 1);
        }
        else
        {
            var value:Int = punter;
            new HsvColor(
                FloatTools.lerp(
                    (value / 2.779) % 1,
                    0.15,
                    0.95
                ),
                FloatTools.lerp(
                    (value / 4.679) % 1,
                    0.65,
                    0.85
                ),
                FloatTools.lerp(
                    (value / 3.879) % 1,
                    0.65,
                    0.85
                )
            );
        }
    }
    
    public static inline function lerp(rate:Float, from:Float, to:Float):Float
    {
        return from * (1 - rate) + to * rate;
    }
    
    public static inline function inverseLerp(value:Float, from:Float, to:Float):Float
    {
        return (value - from) / (to - from);
    }
}
