// Generated by Haxe 3.4.2 (git build master @ 890f8c7)

#pragma warning disable 109, 114, 219, 429, 168, 162
namespace game {
	public class River : global::haxe.lang.HxObject {
		
		public River(global::haxe.lang.EmptyObject empty) {
		}
		
		
		public River(global::game.Game game1, int id, int a, int b) {
			global::game.River.__hx_ctor_game_River(this, game1, id, a, b);
		}
		
		
		public static void __hx_ctor_game_River(global::game.River __hx_this, global::game.Game game1, int id, int a, int b) {
			__hx_this.id = id;
			__hx_this.game = game1;
			__hx_this.a = a;
			__hx_this.b = b;
			__hx_this.owner = global::game._PunterId.PunterId_Impl_.NotFound;
		}
		
		
		public int id;
		
		public global::game.Game game;
		
		public int a;
		
		public int b;
		
		public int owner;
		
		public virtual int getAnother(int source) {
			if (( this.a == source )) {
				return this.b;
			}
			else {
				return this.a;
			}
			
		}
		
		
		public override double __hx_setField_f(string field, int hash, double @value, bool handleProperties) {
			unchecked {
				switch (hash) {
					case 947296307:
					{
						this.owner = ((int) (@value) );
						return @value;
					}
					
					
					case 98:
					{
						this.b = ((int) (@value) );
						return @value;
					}
					
					
					case 97:
					{
						this.a = ((int) (@value) );
						return @value;
					}
					
					
					case 23515:
					{
						this.id = ((int) (@value) );
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
					case 947296307:
					{
						this.owner = ((int) (global::haxe.lang.Runtime.toInt(@value)) );
						return @value;
					}
					
					
					case 98:
					{
						this.b = ((int) (global::haxe.lang.Runtime.toInt(@value)) );
						return @value;
					}
					
					
					case 97:
					{
						this.a = ((int) (global::haxe.lang.Runtime.toInt(@value)) );
						return @value;
					}
					
					
					case 1147073522:
					{
						this.game = ((global::game.Game) (@value) );
						return @value;
					}
					
					
					case 23515:
					{
						this.id = ((int) (global::haxe.lang.Runtime.toInt(@value)) );
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
					case 1119591629:
					{
						return ((global::haxe.lang.Function) (new global::haxe.lang.Closure(this, "getAnother", 1119591629)) );
					}
					
					
					case 947296307:
					{
						return this.owner;
					}
					
					
					case 98:
					{
						return this.b;
					}
					
					
					case 97:
					{
						return this.a;
					}
					
					
					case 1147073522:
					{
						return this.game;
					}
					
					
					case 23515:
					{
						return this.id;
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
					case 947296307:
					{
						return ((double) (this.owner) );
					}
					
					
					case 98:
					{
						return ((double) (this.b) );
					}
					
					
					case 97:
					{
						return ((double) (this.a) );
					}
					
					
					case 23515:
					{
						return ((double) (this.id) );
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
					case 1119591629:
					{
						return this.getAnother(((int) (global::haxe.lang.Runtime.toInt(dynargs[0])) ));
					}
					
					
					default:
					{
						return base.__hx_invokeField(field, hash, dynargs);
					}
					
				}
				
			}
		}
		
		
		public override void __hx_getFields(global::Array<object> baseArr) {
			baseArr.push("owner");
			baseArr.push("b");
			baseArr.push("a");
			baseArr.push("game");
			baseArr.push("id");
			base.__hx_getFields(baseArr);
		}
		
		
	}
}

