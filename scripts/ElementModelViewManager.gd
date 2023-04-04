extends Node3D

class_name ElementModelViewManager

@export_range(0.0, 180.0, 1.0, "deg") var rotation_range = 75.0
@export var custom_things: Array[Node3D]

@onready var window_res = Vector2(get_window().size)
@onready var base_node: MeshInstance3D = $Base
@onready var symbol_text: MeshInstance3D = get_node("Base/SymbolText")
@onready var name_text: MeshInstance3D = get_node("Base/NameText")
@onready var atomic_mass_text: MeshInstance3D = get_node("Base/AtomicMassText")
@onready var atomic_number_text: MeshInstance3D = get_node("Base/AtomicNumberText")
@onready var alternative_mesh: Node3D = get_node("Alternative")

var hue_cycle: float = 0.0
var rotation_enabled: bool = true

func _ready():
	pass

func update_colors(color: Color):
	(symbol_text.material_override as StandardMaterial3D).albedo_color.r = color.r
	(symbol_text.material_override as StandardMaterial3D).albedo_color.g = color.g
	(symbol_text.material_override as StandardMaterial3D).albedo_color.b = color.b
	
	(base_node.material_override as StandardMaterial3D).albedo_color.r = color.lightened(.75).r
	(base_node.material_override as StandardMaterial3D).albedo_color.g = color.lightened(.75).g
	(base_node.material_override as StandardMaterial3D).albedo_color.b = color.lightened(.75).b

func update_texts(data: PTableManager.ElementData):
	swap_to_alternative(false)
	
	if data.name == "Obamium":
		swap_to_alternative(true)
		update_alternative_mesh(custom_things[0])
	
	(symbol_text.mesh as TextMesh).text = data.symbol
	(name_text.mesh as TextMesh).text = data.name
	(atomic_mass_text.mesh as TextMesh).text = str(data.atomic_mass)
	(atomic_number_text.mesh as TextMesh).text = str(data.atomic_number)

func update_rotation():
	var mouse_pos = get_window().get_mouse_position()
	var uv = (mouse_pos / window_res - Vector2(.5, .5))
	
	uv.x = clampf(uv.x, -.5, .5)
	uv.y = clampf(uv.y, -.5, .5)
	
	base_node.rotation_degrees.x = rotation_range * uv.y + 90
	base_node.rotation_degrees.y = rotation_range * uv.x

func update_alternative_mesh(new_node: Node3D):
	alternative_mesh.get_children().all(func(e): alternative_mesh.remove_child(e))
	alternative_mesh.add_child(new_node)
	
func swap_to_alternative(alternative: bool):
	if alternative:
		alternative_mesh.show()
		base_node.hide()
	else:
		alternative_mesh.hide()
		base_node.show()
	

func _process(delta):
	if (name_text.mesh as TextMesh).text == "MLGium":
		update_colors(Color.from_hsv(hue_cycle, 1., 1.))
		hue_cycle += delta
		if hue_cycle > 1.: hue_cycle = 0.0
	
	if rotation_enabled: update_rotation()
	
