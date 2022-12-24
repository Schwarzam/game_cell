extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	EntitiesControler.connect("ended_game", self, "_on_ended_game")
	
	EntitiesControler.startGame()
	pass # Replace with function body.
	
func _on_ended_game():
	print("Ended game")
	queue_free()
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
