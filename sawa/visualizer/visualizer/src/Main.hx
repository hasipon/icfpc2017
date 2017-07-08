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
	static function main() 
    {
		ReactDOM.render(
            React.createElement(
                RootView,
                {
                    context: new RootContext(),
                }
            ), 
            Browser.document.getElementById('application')
        );
	}	
}
