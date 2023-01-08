extends Spatial

onready var Entities = $NavigationMeshInstance/Entities

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func spawn(node):
	Entities.add_child(node)


func get_player_node(id):
	Entities.get_node(str(id))
