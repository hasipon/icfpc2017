package component.root;
import core.PlayingState;
import game.PunterId;
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
                ("スコア：":Dynamic)
            ].concat(
                [
                    for (scoreStruct in props.context.scores)
                    {
                        var data = "";
                        if ((scoreStruct.punter:Int) < props.context.punterNames.length)
                        {
                            data += props.context.punterNames[(scoreStruct.punter:Int)];
                        }
                        else
                        {
                            data += scoreStruct.punter + "番";
                        }
                        
                        if (context.you == scoreStruct.punter) data += "(選択中)";
                        data += ":";
                        data += scoreStruct.score;
                        data += ", ";
                        "font".createElement(
                            {
                                color : {
                                    var color = PixiView.getColor(scoreStruct.punter, context.you);
                                    "#" + color.toRgbHexString();
                                },
                                onClick: onClick.bind(scoreStruct.punter),
                            },
                            data
                        );
                    }
                ]
            )
        );
    }
    
    function onClick(index:PunterId)
    {
        props.context.changeYou(index);
    }
}

typedef PlayingStateControlProps = 
{
    context : PlayingState,
}
