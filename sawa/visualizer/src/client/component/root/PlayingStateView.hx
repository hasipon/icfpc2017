package component.root;
import core.PlayingState;
import js.html.Event;
import react.ReactComponent.ReactComponentOfProps;
import react.ReactComponent.ReactElement;

class PlayingStateView extends ReactComponentOfProps<PlayingStateProps>
{
    public function new (props) { super(props); }
    
    override public function render():ReactElement
    {
        var context = props.context;
        
        return "div".createElement(
            {
                className: "result",
            },
            [
                "button".createElement(
                    {
                        onClick: onGotoTopClick
                    },
                    "|<"
                ), 
                "button".createElement(
                    {
                        onClick: onUndoClick
                    },
                    "<"
                ), 
                "button".createElement(
                    {
                        onClick: onTogglePlayingClick
                    },
                    if (context.playing) "■" else "▶"
                ),
                "button".createElement(
                    {
                        onClick: onDoClick
                    },
                    ">"
                ), 
                "button".createElement(
                    {
                        onClick: onGotoEndClick
                    },
                    ">|"
                ), 
                "input".createElement(
                    {
                        onChange: onChangeIndex,
                        value: context.currentIndex,
                    }
                ), 
                " / " + context.moves.length, 
                " FPS:",
                "input".createElement(
                    {
                        onChange: onChangeFps,
                        value: context.parent.framePerSec,
                    }
                ), 
            ]
        );
    }
    
    function onChangeFps(e:Event) 
    {
        props.context.parent.changeFps(untyped e.target.value);
    }
    
    function onGotoTopClick() 
    {
        props.context.gotoTop();
    }
    
    function onGotoEndClick() 
    {
        props.context.gotoEnd();
    }
    function onChangeIndex()
    {
        props.context.changeIndex(Std.parseInt(untyped e.target.value));
    }

    private function onTogglePlayingClick()
    {
        props.context.togglePlaying();
    }
    
    private function onDoClick()
    {
        props.context.doMove();
    }
    
    private function onUndoClick()
    {
        props.context.undoMove();
    }
}

typedef PlayingStateProps = 
{
    context : PlayingState,
}