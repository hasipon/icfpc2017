package;

import component.root.RootView;
import core.RootContext;
import js.Browser;
import react.React;
import react.ReactComponent.ReactElement;
import react.ReactDOM;
import react.ReactEvent;
import react.ReactTools;

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
}
