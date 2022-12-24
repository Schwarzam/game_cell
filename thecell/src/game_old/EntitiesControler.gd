extends Node

signal ended_game

var GAME_RUNNING = false
var entities = {}
var new_entity = {
	life = 20
}
var user

	
func _ready():
	
	pass # Replace with function body.

func startGame():
	var player = load("res://Player/PlayerScene.tscn").instance()
	player.heroSelected = "Ray"
	player.inputs = true
	get_tree().get_root().get_node("Game").add_child(player)


func _process(delta):
	for i in entities:
		pass
		#print(entities[i])
	
	if GAME_RUNNING and entities.size() <= 0:
		end_game()


func createPlayerChar(hero):
	var mySprite = load("res://Player/PlayerScene.tscn").instance()
	
	pass
	
	
func _register_user(user):
	self.user = user

func _register_enitity(id):
	self.entities[id] = new_entity.duplicate()

#activated by entity once damaged
func _on_damage(id):
	if id in entities:
		entities[id].life -= 10
		if entities[id].life <= 0:
			var node = instance_from_id(id)
			node.on_death()
			entities.erase(id)
		

func end_game():
	GAME_RUNNING = false
	EntitiesControler.emit_signal("ended_game")
	get_tree().change_scene("res://Game/Menu.tscn")
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass