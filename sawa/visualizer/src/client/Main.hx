package;

import component.root.RootView;
import core.RootContext;
import js.Browser;
import react.React;
import react.ReactDOM;
import react.ReactTools;

/**
 * ...
 * @author shohei909
 */
class Main 
{
    public static var pixiView:PixiView;
    
	static function main() 
    {
        var rootContext = new RootContext();
        
		ReactDOM.render(
            React.createElement(
                RootView,
                {
                    context: rootContext,
                }
            ),
            Browser.document.getElementById('control')
        );
        
        new PixiView(rootContext);
	}
}
