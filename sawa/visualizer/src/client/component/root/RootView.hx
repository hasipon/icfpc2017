package component.root;
import core.RootContext;
import haxe.ds.Option;
import js.html.Event;
import react.React;
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
                            }   ,
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
                    ", 最大スコア理論値 : " + props.context.game.maxScore
                ),
                "div".createElement(
                    {},
                    [
                        "textarea".createElement(
                            {
                                placeholder: "ログ",
                                onChange: onChangeLog,
                                value: props.context.log,
                            },
                            []
                        ),
                        "button".createElement(
                            {
                                onClick: onClickLog
                            },
                            ["ログ再生"]
                        )
                    ]
                ),
                
                "div".createElement(
                    {},
                    switch (props.context.playingState)
                    {
                        case Option.None: [];
                        
                        case Option.Some(playingState):
                            [
                                React.createElement(
                                    PlayingStateView,
                                    {
                                        context: playingState,
                                    }
                                ),
                                React.createElement(
                                    PlayingStateControlView,
                                    {
                                        context: playingState,
                                    }
                                )
                            ];
                    }
                ),
                "div".createElement(
                    {},
                    if (props.context.warning == null) "" else "エラー：" + props.context.warning
                ),
                "div".createElement(
                    {},
                    "version : 2.6"
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
    
    private function onClickLog(e:Event):Void
    {
        props.context.execLog();
    }
    
    private function onChangeLog(e:Event):Void
    {
        props.context.changeLog(untyped e.target.value);
    }
}

typedef RootProps = 
{
    context: RootContext,
}
