extends Spatial


func _ready():
	global._send_P2P_Packet(0, {"test": "teste"})
	
