package component.root;
import core.PlayingState;
import react.ReactComponent.ReactComponentOfProps;
import react.ReactComponent.ReactElement;

class PlayingStateControlView extends ReactComponentOfProps<PlayingStateControlProps>
{
    override public function render():ReactElement
    {
        var context = props.context;
        
        return "div".createElement(
            {
                className: "result",
            },
            [
                {
                    var data = "スコア：";
                    for (scoreStruct in props.context.scores)
                    {
                        data += scoreStruct.punter + "番";
                        if (context.you == scoreStruct.punter) data += "(あなた)";
                        data += ":";
                        data += scoreStruct.score;
                        data += ", ";
                    }
                    data;
                }
            ]
        );
    }
}

typedef PlayingStateControlProps = 
{
    context : PlayingState,
}
