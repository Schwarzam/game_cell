extends Node

# Steam variables
var IS_OWNED: bool = false
var IS_ONLINE: bool = false
var IS_FREE_WEEKEND: bool = false
var STEAM_ID: int = 0
var STEAM_NAME: String = ""

const PACKET_READ_LIMIT: int = 32
var STEAM_USERNAME: String = ""
var LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var DATA
var LOBBY_VOTE_KICK: bool = false
var LOBBY_MAX_MEMBERS: int = 6
enum LOBBY_AVAILABILITY {PRIVATE, FRIENDS, PUBLIC, INVISIBLE}

func _ready() -> void:
	if OS.get_name() != "OSX":
		var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()
	# Initialize Steam
	var INIT: Dictionary = Steam.steamInit(false)
	print("[STEAM] Did Steam initialize?: "+str(INIT))
	if INIT['status'] != 1:
		# If Steam fails to start up, shut down the app
		print("[STEAM] Failed to initialize Steam. "+str(INIT['verbal'])+" Shutting down...")
#		get_tree().quit()
	#Is the user online?
	IS_ONLINE = Steam.loggedOn()
	
	# Get the user's Stean name and ID
	STEAM_ID = Steam.getSteamID()
	STEAM_NAME = Steam.getPersonaName()
	# Is this app owned or is it a free weekend?
	IS_OWNED = Steam.isSubscribed()
	IS_FREE_WEEKEND = Steam.isSubscribedFromFreeWeekend()
	
	Steam.connect("lobby_created", self, "_on_Lobby_Created")
	Steam.connect("lobby_match_list", self, "_on_Lobby_Match_List")
	Steam.connect("lobby_joined", self, "_on_Lobby_Joined")
	Steam.connect("lobby_chat_update", self, "_on_Lobby_Chat_Update")
	Steam.connect("lobby_message", self, "_on_Lobby_Message")
	Steam.connect("lobby_data_update", self, "_on_Lobby_Data_Update")
	Steam.connect("lobby_invite", self, "_on_Lobby_Invite")
	Steam.connect("join_requested", self, "_on_Lobby_Join_Requested")
	Steam.connect("persona_state_change", self, "_on_Persona_Change")
	Steam.connect("p2p_session_request", self, "_on_P2P_Session_Request")
	Steam.connect("p2p_session_connect_fail", self, "_on_P2P_Session_Connect_Fail")
	
	_on_Open_Lobby_List_pressed()


func _process(_delta: float) -> void:
	# Get callbacks
	Steam.run_callbacks()
	
	# If the player is connected, read packets
	if LOBBY_ID > 0:
		_read_P2P_Packet()

func _create_Lobby() -> void:
	# Make sure a lobby is not already set
	if LOBBY_ID == 0:
		Steam.createLobby(LOBBY_AVAILABILITY.PUBLIC, LOBBY_MAX_MEMBERS)
	
func _on_Open_Lobby_List_pressed() -> void:
	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(3)

	print("Requesting a lobby list")
	Steam.requestLobbyList()

func _on_Lobby_Match_List(lobbies: Array) -> void:
	print("Got lobby list")
	for LOBBY in lobbies:
		# Pull lobby data from Steam, these are specific to our example
		var LOBBY_NAME: String = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_MODE: String = Steam.getLobbyData(LOBBY, "mode")
		print(LOBBY_NAME, LOBBY_MODE)
		# Get the current number of members
		var LOBBY_NUM_MEMBERS: int = Steam.getNumLobbyMembers(LOBBY)

		# Create a button for the lobby
		var LOBBY_BUTTON: Button = Button.new()
		#LOBBY_BUTTON.set_text("Lobby ", LOBBY, ": ", LOBBY_NAME, " [", LOBBY_MODE, "] - ", LOBBY_NUM_MEMBERS, " Player(s)")
		LOBBY_BUTTON.set_size(Vector2(800, 50))
		LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
		LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])

		# Add the new lobby to the list
		#$"Lobby Panel/Panel/Scroll/VBox".add_child(LOBBY_BUTTON)

func _on_Lobby_Created(connect: int, lobby_id: int) -> void:
	if connect == 1:
		# Set the lobby ID
		LOBBY_ID = lobby_id
		print("Created a lobby: "+str(LOBBY_ID))

		# Set this lobby as joinable, just in case, though this should be done by default
		Steam.setLobbyJoinable(LOBBY_ID, true)

		# Set some lobby data
		Steam.setLobbyData(lobby_id, "name", "The Cell - Normal Lobby")
		Steam.setLobbyData(lobby_id, "mode", "Normal")

		# Allow P2P connections to fallback to being relayed through Steam if needed
		var RELAY: bool = Steam.allowP2PPacketRelay(true)
		print("Allowing Steam to be relay backup: "+str(RELAY))

func _read_P2P_Packet() -> void:
	var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)

	# There is a packet
	if PACKET_SIZE > 0:
		var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)

		if PACKET.empty() or PACKET == null:
			print("WARNING: read an empty packet with non-zero size!")

		# Get the remote user's ID
		var PACKET_SENDER: int = PACKET['steam_id_remote']

		# Make the packet data readable
		var PACKET_CODE: PoolByteArray = PACKET['data']
		# Decompress the array before turning it into a useable dictionary
		var READABLE: Dictionary = bytes2var(PACKET_CODE.decompress_dynamic(-1, File.COMPRESSION_GZIP))

		# Print the packet to output
		print("Packet: "+str(READABLE))

		# Append logic here to deal with packet data
	
func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
	# Set the send_type and channel
	var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
	var CHANNEL: int = 0

	# Create a data array to send the data through
	var DATA: PoolByteArray
	# Compress the PoolByteArray we create from our dictionary  using the GZIP compression method
	var COMPRESSED_DATA: PoolByteArray = var2bytes(packet_data).compress(File.COMPRESSION_GZIP)
	DATA.append_array(COMPRESSED_DATA)

	# If sending a packet to everyone
	if target == 0:
		# If there is more than one user, send packets
		if LOBBY_MEMBERS.size() > 1:
			# Loop through all members that aren't you
			for MEMBER in LOBBY_MEMBERS:
				if MEMBER['steam_id'] != STEAM_ID:
					Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
	# Else send it to someone specific
	else:
		Steam.sendP2PPacket(target, DATA, SEND_TYPE, CHANNEL)
