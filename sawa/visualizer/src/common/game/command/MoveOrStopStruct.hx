package game.command;

typedef MoveOrStopStruct ={
    move: {
        moves: Array<MoveStruct>,
    },
    stop: {
        moves: Array<MoveStruct>,
        scores: Array<ScoreStruct>,
    }
}