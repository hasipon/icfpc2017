package;

import component.root.RootView;
import core.RootContext;
import haxe.Timer;
import haxe.ds.Option;
import js.Browser;
import js.html.KeyEvent;
import js.html.KeyboardEvent;
import react.React;
import react.ReactDOM;
/**
 * ...
 * @author shohei909
 */
class Main 
{
    private static var rootPixi:PixiView;
    private static var rootContext:RootContext;
    
	static function main() 
    {
        rootContext = new RootContext();
        rootPixi = new PixiView(rootContext);
        
        rootContext.updateUi = render;
        rootContext.updatePixi = rootPixi.update;
        
        render();
        
        update();

        Browser.document.onkeydown = onKeyDown;
	}
    
    private static function update() 
    {
        rootContext.onFrame(15);
        Timer.delay(update, 15);
    }
    
    public static function render():Void
    {
        ReactDOM.render(
            React.createElement(
                RootView,
                {
                    context: rootContext,
                }
            ),
            Browser.document.getElementById('control')
        );
    }
    
    public static function onKeyDown(e:KeyboardEvent):Bool
    {
        if (e.altKey || e.ctrlKey || e.shiftKey) { return true; } 
        return switch (e.keyCode)
        {
            case KeyboardEvent.DOM_VK_A:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.gotoTop();
                    case Option.None:
                }
                true;
                
            case KeyboardEvent.DOM_VK_S | KeyboardEvent.DOM_VK_LEFT:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.undoMove();
                    case Option.None:
                }
                true;
                
            case KeyboardEvent.DOM_VK_D:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.togglePlaying();
                    case Option.None:
                }
                true;
                
            case KeyboardEvent.DOM_VK_F | KeyboardEvent.DOM_VK_RIGHT:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.doMove();
                    case Option.None:
                }
                true;
                
            case KeyboardEvent.DOM_VK_G:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.gotoEnd();
                    case Option.None:
                }
                true;
                
            case KeyboardEvent.DOM_VK_Z:
                switch (rootContext.playingState)
                {
                    case Option.Some(s): s.playing = true;
                    case Option.None:
                }
                rootContext.framePerSec = -rootContext.framePerSec;
                rootContext.updateUi();
                true;
                
            case KeyboardEvent.DOM_VK_L:
                rootContext.execLog();
                true;
                
            case _:
                true;
        }
    }
}
