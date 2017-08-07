package game.command;

typedef MoveStruct = {
    ?claim: ClaimStruct,
    ?pass: { punter: PunterId },
    ?splurge: SplurgeStruct,
    ?option: ClaimStruct,
}
