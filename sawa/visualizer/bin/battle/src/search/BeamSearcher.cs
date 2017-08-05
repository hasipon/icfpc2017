// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
namespace search {
	public class BeamSearcher : global::haxe.lang.HxObject, global::search.Searcher {
		
		public BeamSearcher(global::haxe.lang.EmptyObject empty) {
		}
		
		
		public BeamSearcher(double timeout, global::eval.Evaluter evaluter) {
			global::search.BeamSearcher.__hx_ctor_search_BeamSearcher(this, timeout, evaluter);
		}
		
		
		public static void __hx_ctor_search_BeamSearcher(global::search.BeamSearcher __hx_this, double timeout, global::eval.Evaluter evaluter) {
			__hx_this.evaluter = evaluter;
			__hx_this.timeout = timeout;
		}
		
		
		public double timeout;
		
		public global::eval.Evaluter evaluter;
		
		public virtual object getMove(global::game.Game game, int punter) {
			unchecked {
				global::Array<object> livingRivers = game.getLivingRivers();
				global::Array<object> moves = new global::Array<object>(new object[]{});
				{
					int _g = 0;
					while (( _g < livingRivers.length )) {
						global::game.River river = ((global::game.River) (livingRivers[_g]) );
						 ++ _g;
						object __temp_stmt3 = null;
						{
							object __temp_odecl1 = new global::haxe.lang.DynamicObject(new int[]{}, new object[]{}, new int[]{116192081, 842117339, 1860705976}, new double[]{((double) (river.b) ), ((double) (river.a) ), ((double) (punter) )});
							__temp_stmt3 = new global::haxe.lang.DynamicObject(new int[]{1213755260}, new object[]{__temp_odecl1}, new int[]{}, new double[]{});
						}
						
						moves.push(new global::search.MoveAndScore(((object) (__temp_stmt3) )));
					}
					
				}
				
				object __temp_stmt4 = null;
				{
					object __temp_odecl2 = new global::haxe.lang.DynamicObject(new int[]{}, new object[]{}, new int[]{1860705976}, new double[]{((double) (punter) )});
					__temp_stmt4 = new global::haxe.lang.DynamicObject(new int[]{1246880977}, new object[]{__temp_odecl2}, new int[]{}, new double[]{});
				}
				
				moves.push(new global::search.MoveAndScore(((object) (__temp_stmt4) )));
				{
					int _g1 = 0;
					while (( _g1 < moves.length )) {
						global::search.MoveAndScore move = ((global::search.MoveAndScore) (moves[_g1]) );
						 ++ _g1;
						game.addMove(move.move);
						move.score = this.evaluter.eval(game);
						game.undoMove();
					}
					
				}
				
				int id = punter;
				moves.sort(new global::search.BeamSearcher_getMove_45__Fun(id));
				global::haxe.Log.trace.__hx_invoke2_o(default(double), global::haxe.lang.Runtime.concat("score max:", global::haxe.lang.Runtime.toString(((global::search.MoveAndScore) (moves[0]) ).score.getFor(punter))), default(double), new global::haxe.lang.DynamicObject(new int[]{302979532, 1547539107, 1648581351}, new object[]{"getMove", "search.BeamSearcher", "BeamSearcher.hx"}, new int[]{1981972957}, new double[]{((double) (46) )}));
				global::haxe.Log.trace.__hx_invoke2_o(default(double), global::haxe.lang.Runtime.concat("score min:", global::haxe.lang.Runtime.toString(((global::search.MoveAndScore) (moves[( moves.length - 1 )]) ).score.getFor(punter))), default(double), new global::haxe.lang.DynamicObject(new int[]{302979532, 1547539107, 1648581351}, new object[]{"getMove", "search.BeamSearcher", "BeamSearcher.hx"}, new int[]{1981972957}, new double[]{((double) (47) )}));
				return ((global::search.MoveAndScore) (moves[0]) ).move;
			}
		}
		
		
		public override double __hx_setField_f(string field, int hash, double @value, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 1492589217:
					{
						this.timeout = ((double) (@value) );
						return @value;
					}
					
					
					default:
					{
						return base.__hx_setField_f(field, hash, @value, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_setField(string field, int hash, object @value, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 22986280:
					{
						this.evaluter = ((global::eval.Evaluter) (@value) );
						return @value;
					}
					
					
					case 1492589217:
					{
						this.timeout = ((double) (global::haxe.lang.Runtime.toDouble(@value)) );
						return @value;
					}
					
					
					default:
					{
						return base.__hx_setField(field, hash, @value, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_getField(string field, int hash, bool throwErrors, bool isCheck, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 451121863:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "getMove", 451121863)) );
					}
					
					
					case 22986280:
					{
						return this.evaluter;
					}
					
					
					case 1492589217:
					{
						return this.timeout;
					}
					
					
					default:
					{
						return base.__hx_getField(field, hash, throwErrors, isCheck, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override double __hx_getField_f(string field, int hash, bool throwErrors, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 1492589217:
					{
						return this.timeout;
					}
					
					
					default:
					{
						return base.__hx_getField_f(field, hash, throwErrors, handleProperties);
					}
					
				}
				
			}
		}
		
		
		public override object __hx_invokeField(string field, int hash, global::Array dynargs) {
			unchecked {
				switch (hash) {
					case 451121863:
					{
						return this.getMove(((global::game.Game) (dynargs[0]) ), ((int) (global::haxe.lang.Runtime.toInt(dynargs[1])) ));
					}
					
					
					default:
					{
						return base.__hx_invokeField(field, hash, dynargs);
					}
					
				}
				
			}
		}
		
		
		public override void __hx_getFields(global::Array<object> baseArr) {
			baseArr.push("evaluter");
			baseArr.push("timeout");
			base.__hx_getFields(baseArr);
		}
		
		
	}
}



#pragma warning disable 109, 114, 219, 429, 168, 162
namespace search {
	public class BeamSearcher_getMove_45__Fun : global::haxe.lang.Function {
		
		public BeamSearcher_getMove_45__Fun(int id) : base(2, 1) {
			this.id = id;
		}
		
		
		public override double __hx_invoke2_f(double __fn_float1, object __fn_dyn1, double __fn_float2, object __fn_dyn2) {
			global::search.MoveAndScore b = ( (( __fn_dyn2 == global::haxe.lang.Runtime.undefined )) ? (((global::search.MoveAndScore) (((object) (__fn_float2) )) )) : (((global::search.MoveAndScore) (__fn_dyn2) )) );
			global::search.MoveAndScore a = ( (( __fn_dyn1 == global::haxe.lang.Runtime.undefined )) ? (((global::search.MoveAndScore) (((object) (__fn_float1) )) )) : (((global::search.MoveAndScore) (__fn_dyn1) )) );
			return ((double) (global::search.MoveAndScore.compare(this.id, a, b)) );
		}
		
		
		public int id;
		
	}
}


