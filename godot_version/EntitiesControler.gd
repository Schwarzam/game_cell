extends Node

var entities = {}
var new_entity = {
	life = 20
}
var user


func _initialize_Steam() -> void:
	#var INIT: Dictionary = Steam.steamInit()
	#print("Did Steam initialize?: "+str(INIT))
	print(Steam)
	
func _ready():
	_initialize_Steam()
	
	
	pass # Replace with function body.

func _process(delta):
	for i in entities:
		pass
		#print(entities[i])
	
func _register_user(user):
	self.user = user

func _register_enitity(id):
	self.entities[id] = new_entity.duplicate()

#activated by entity once damaged
func _on_damage(id):
	entities[id].life -= 10
	if entities[id].life <= 0:
		var node = instance_from_id(id)
		node.on_death()
		

func print_tei():
	pass
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
