package component.root;
import core.RootContext;
import js.html.Event;
import react.ReactComponent;
import react.ReactComponent.ReactComponentOfProps;

class RootView extends ReactComponentOfProps<RootProps>
{
    public function new (props) { super(props); }
    
    override public function render():ReactElement
    {
        var i = 0;
        return "div".createElement(
            {
                className: "root",
            },
            [
                "div".createElement(
                    {},
                    [
                        "select".createElement(
                            {
                                name: "map",
                                size: "12",
                                onChange: onSelect,
                            },
                            [for (mapName in props.context.mapNames){
                                "option".createElement(
                                    {
                                        value: mapName,
                                        selected: (i++ == props.context.selectedIndex)
                                    },
                                    mapName
                                );
                            }]
                        ),
                        "button".createElement(
                            {
                                onClick: onClick
                            },
                            ["表示"]
                        ), 
                    ]
                ),
                
                "div".createElement(
                    {},
                    "site : " + props.context.game.siteCount +
                    ", mine : " + props.context.game.mineCount +
                    ", river : " + props.context.game.riverCount +
                    ", 最大スコア : " + props.context.game.maxScore
                ),
                
                "div".createElement(
                    {},
                    "version : 1.2"
                ),
            ]
        );
    }
    
    private function onClick(e:Event):Void
    {
        props.context.exec();
    }
    
    private function onSelect(e:Event):Void
    {
        props.context.select(untyped e.target.selectedIndex);
    }
    
}

typedef RootProps = 
{
    context: RootContext,
}
