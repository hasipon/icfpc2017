package;
import core.RootContext;
import js.Browser;
import pixi.core.graphics.Graphics;
import pixi.core.sprites.Sprite;
import pixi.core.textures.Texture;
import pixi.plugins.app.Application;

class PixiView extends Application 
{
    static var WIDTH = 800;
    static var HEIGHT = 650;
    
    var _graphic:Graphics;
    var rootContext:RootContext;
    
	public function new(rootContext:RootContext) {
		super();
        this.rootContext = rootContext;

		position = Application.POSITION_FIXED;
		width = WIDTH;
		height = HEIGHT;
		backgroundColor = 0x006666;
		transparent = true;
		antialias = false;
		onUpdate = updateHandler;
		super.start("auto", Browser.document.getElementById("pixi"));

		_graphic = new Graphics();
		_graphic.beginFill(0xF9F9F9, 1);
		_graphic.drawRect(0, 0, WIDTH, HEIGHT);
		_graphic.endFill();

		stage.addChild(_graphic);
	}
    
    function updateHandler(t:Float):Void
    {
        trace("time : " + t);
    }
}
