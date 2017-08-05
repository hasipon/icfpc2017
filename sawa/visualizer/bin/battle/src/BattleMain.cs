// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
public class BattleMain : global::haxe.lang.HxObject {
	
	public static void Main(){
		global::cs.Boot.init();
		{
			global::BattleMain.main();
		}
	}
	static BattleMain() {
		unchecked {
			global::BattleMain.PUNTERS = 4;
			global::BattleMain.MAPS = "../../maps";
		}
	}
	
	
	public BattleMain(global::haxe.lang.EmptyObject empty) {
	}
	
	
	public BattleMain(string mapPath) {
		global::BattleMain.__hx_ctor__BattleMain(this, mapPath);
	}
	
	
	public static void __hx_ctor__BattleMain(global::BattleMain __hx_this, string mapPath) {
		unchecked {
			global::haxe.Log.trace.__hx_invoke2_o(default(double), mapPath, default(double), new global::haxe.lang.DynamicObject(new int[]{302979532, 1547539107, 1648581351}, new object[]{"new", "BattleMain", "BattleMain.hx"}, new int[]{1981972957}, new double[]{((double) (29) )}));
			global::game.Game game1 = new global::game.Game();
			object map = new global::haxe.format.JsonParser(((string) (global::sys.io.File.getContent(mapPath)) )).parseRec();
			global::eval.Evaluter evaluter = new global::eval.Evaluter(((global::Array<double>) (global::eval.Evaluter.MS_TABLE) ));
			global::search.BeamSearcher sercher = new global::search.BeamSearcher(0.1, evaluter);
			game1.setupMap(map);
			game1.setupPunters(global::BattleMain.PUNTERS);
			{
				int _g1 = 0;
				int _g = game1.riverCount;
				while (( _g1 < _g )) {
					int i = _g1++;
					int punter = global::game._PunterId.PunterId_Impl_._new(( i % global::BattleMain.PUNTERS ));
					object move = sercher.getMove(game1, punter);
					game1.addMove(move);
					global::haxe.Log.trace.__hx_invoke2_o(default(double), global::haxe.lang.Runtime.concat("move", global::haxe.lang.Runtime.toString(i)), default(double), new global::haxe.lang.DynamicObject(new int[]{302979532, 1547539107, 1648581351}, new object[]{"new", "BattleMain", "BattleMain.hx"}, new int[]{1981972957}, new double[]{((double) (42) )}));
				}
				
			}
			
		}
	}
	
	
	public static int PUNTERS;
	
	public static string MAPS;
	
	public static void main() {
		unchecked {
			global::Array<object> maps = new global::Array<object>(new object[]{global::haxe.lang.Runtime.concat(global::BattleMain.MAPS, "/circle.json")});
			{
				int _g = 0;
				while (( _g < 200 )) {
					int i = _g++;
					new global::BattleMain(global::haxe.lang.Runtime.toString(maps[((int) (( maps.length * global::Math.rand.NextDouble() )) )]));
					global::haxe.Log.trace.__hx_invoke2_o(default(double), global::haxe.lang.Runtime.concat("GAME\u7d42\u4e86:", global::haxe.lang.Runtime.toString(i)), default(double), new global::haxe.lang.DynamicObject(new int[]{302979532, 1547539107, 1648581351}, new object[]{"main", "BattleMain", "BattleMain.hx"}, new int[]{1981972957}, new double[]{((double) (21) )}));
				}
				
			}
			
			global::Sys.getChar(false);
		}
	}
	
	
}

