package component.root;
import core.RootContext;
import react.ReactComponent;
import react.ReactComponent.ReactComponentOfProps;

class RootView extends ReactComponentOfProps<RootProps>
{
    public function new (props) { super(props); }
    
    override public function render():ReactElement
    {
        return "div".createElement(
            {
                className: "root",
            },
            [
                "",
            ]
        );
    }
}

typedef RootProps = 
{
    context: RootContext,
}
