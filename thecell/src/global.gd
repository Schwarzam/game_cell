extends Node
#################################################
# GODOTSTEAM EXAMPLE PROJECT v2.1

# Set up some global Steam variables
var IS_OWNED: bool = false
var IS_ONLINE: bool = false
var STEAM_ID: int = 0
var STEAM_USERNAME: String

# Set up some lobby variables
var LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var DATA
var LOBBY_VOTE_KICK: bool = false
var LOBBY_MAX_MEMBERS: int = 10
enum LOBBY_AVAILABILITY {PRIVATE, FRIENDS, PUBLIC, INVISIBLE}

var host = false

func _ready() -> void:
	# Start Steamworks
	_initialize_Steam()

func reset_lobby():
	LOBBY_ID = 0
	LOBBY_MEMBERS = []
	DATA = null
	LOBBY_VOTE_KICK = false
	LOBBY_MAX_MEMBERS = 10

# Initializing Steamworks
func _initialize_Steam() -> void:
	# Get the initialization dictionary from Steam
	var INIT: Dictionary = Steam.steamInit()
	# If the status isn't one, print out the possible error and quit the program
	if INIT['status'] != 1:
		print("[STEAM] Failed to initialize: "+str(INIT['verbal'])+" Shutting down...")
		get_tree().quit()

	# Acquire information about the user
	IS_ONLINE = Steam.loggedOn()
	STEAM_ID = Steam.getSteamID()
	IS_OWNED = Steam.isSubscribed()
	STEAM_USERNAME = Steam.getPersonaName()
	print("App Id: ", Steam.getAppID())

	# Check if account owns the game
	if IS_OWNED == false:
		print("[STEAM] User does not own this game")
		# Uncomment this line to close the game if the user does not own the game
#		get_tree().quit()

# Process all Steamworks callbacks
func _process(_delta: float) -> void:
	Steam.run_callbacks()

func instance_node(node: Object, parent: Object) -> Object:
	var node_instance = node.instance()
	parent.add_child(node_instance)
	return node_instance

func instance_node_at_location(node: Object, parent: Object, location: Vector3) -> Object:
	var node_instance = instance_node(node, parent)
	node_instance.global_transform.origin = location
	return node_instance

# Send a Steam P2P packet
func _send_P2P_Packet(target: int, packet_data: Dictionary, SEND_TYPE: int = Steam.P2P_SEND_RELIABLE) -> void:
	var CHANNEL: int = 0
	# Create a data array to send the data through
	var PACKET_DATA: PoolByteArray = []
	PACKET_DATA.append_array(var2bytes(packet_data))
	# If sending a packet to everyone
	var SEND_RESPONSE: bool
	if target == 0:
		# If there is more than one user, send packets
		if global.LOBBY_MEMBERS.size() > 1:
			# Loop through all members that aren't you
			for MEMBER in global.LOBBY_MEMBERS:
				if MEMBER['steam_id'] != global.STEAM_ID:
					SEND_RESPONSE = Steam.sendP2PPacket(MEMBER['steam_id'], PACKET_DATA, SEND_TYPE, CHANNEL)
	# Else send the packet to a particular user
	else:
		# Send this packet
		SEND_RESPONSE = Steam.sendP2PPacket(target, PACKET_DATA, SEND_TYPE, CHANNEL)
	# The packets send response is...?
	#print("[STEAM] P2P packet sent successfully? "+str(SEND_RESPONSE)+"\n")


