extends GridContainer

class_name PTableManager

@export var element_colors : JSON
@export var element_data : JSON
@export var element_button_prefab : PackedScene
@export var element_button_theme : Theme
@export var element_scene_prefab : PackedScene

var element_theme: Dictionary
var element_array: Array

signal element_pressed(element_data: Dictionary)

class ElementData:
	var atomic_number: int = 1
	var name: String = "Hydrogen"
	var symbol: String = "H"
	var atomic_mass: float = 1.008
	var description: String
	var relative_pos: Vector2i = Vector2i(1, 1)
	var button_theme: Theme
	var color: Color = Color(.8, .8, .8)
	var shells: Array = [1]
	
	var additional_properties: Dictionary = {}

func generate_theme(from_theme: Theme, element_color: Color) -> Theme:
	var out: Theme = from_theme.duplicate()
	
	out.set_color("font_color", "Button", element_color.lightened(.25))
	out.set_color("font_disabled_color", "Button", element_color)
	out.set_color("font_focus_color", "Button", element_color)
	out.set_color("font_hover_color", "Button", element_color)
	out.set_color("font_hover_pressed_color", "Button", element_color)
	out.set_color("font_pressed_color", "Button", element_color)
	
	for name in out.get_stylebox_list("Button"):
		var stylebox = out.get_stylebox(name, "Button").duplicate() as StyleBoxFlat
		stylebox.border_color = element_color
		stylebox.bg_color = Color(0., 0., 0., 0.)
		out.set_stylebox(name, "Button", stylebox)
	
	# for name in out.get_stylebox_list("Button"):
		# out.get_stylebox(name, "Button").
	return out

func create_element_button(data: ElementData):
	var button = element_button_prefab.instantiate() as Button
	button.text = data.symbol
	
	var label = Label.new()
	label.text = str(data.atomic_number)
	label.label_settings = LabelSettings.new()
	label.label_settings.font_color = data.color
	label.label_settings.font = button.get_theme_font("font", "Button")
	label.position.x += 4
	label.position.y += 3
	
	button.add_child(label)
	
	button.connect("pressed", 
		func():
			var root = get_tree().root
			(root.get_node("Node2D/AnimationPlayer") as AnimationPlayer).play("camera_movement")
			(root.get_node("Node2D/element") as ColorSetup).element_data = data
	)
	
	button.theme = data.button_theme
	# button.flat = true
	
	add_child(button)
	
func determine_max(element_data: Array) -> Vector2i:
	var max = Vector2i(0, 0)
	
	for element in element_data:
		if element.xpos > max.x: max.x = element.xpos
		if element.ypos > max.y: max.y = element.ypos
		
	return max
	
func convert_to_grid(data: Array) -> Array:
	var grid_max = determine_max(data)
	var out = []
	
	out.resize(grid_max.y)
	for col_idx in out.size():
		out[col_idx] = []
		out[col_idx].resize(grid_max.x)
	
	for element in data:
		out[element.ypos - 1][element.xpos - 1] = element
		
	return out

func order_by_atomic_number(data: Array) -> Array:
	var out = []
	out.resize(data.size())
	
	for el in data:
		out[el.number - 1] = el
		
	return out

func get_element_types(data: Array) -> Array:
	var out = []
	
	for element in data:
		if out.has(element.category): continue
		else: out.append(element.category)
		
	return out

func gen_element_data(element: Dictionary) -> ElementData:
	var data = ElementData.new()
	
	data.atomic_number = element.number
	data.name = element.name
	data.symbol = element.symbol
	data.shells = element.shells
	data.atomic_mass = element.atomic_mass
	data.color = Color.from_string(element_colors.data[element.category], Color(.66, .66, .66, 1.0))
	data.button_theme = element_theme[element.category]
	data.relative_pos = Vector2i(element.xpos, element.ypos)
	data.description = element.summary
	
	data.additional_properties["Boiling Point"] = str(element.boil) + " K"
	data.additional_properties["Melting Point"] = str(element.melt) + " K"
	data.additional_properties["Density"] = str(element.density) + "g/L"
	data.additional_properties["Appearance"] = element.appearance
	data.additional_properties["Phase"] = element.phase
	data.additional_properties["Electron Configuration"] = element.electron_configuration_semantic
	data.additional_properties["Electronegativity Pauling"] = str(element.electronegativity_pauling)
	data.additional_properties["Category"] = element.category
	data.additional_properties["Discovered By"] = element.discovered_by
	data.additional_properties["Named By"] = element.named_by
	
	return data

func _ready():
	element_data = element_data
	var elements = element_data.data.elements
	var atomic_sorted_elements = order_by_atomic_number(elements)
	
	var empty_button_theme = Theme.new()
	empty_button_theme.set_color("font_outline_color", "Button", "00000000")
	
	var themes: Dictionary = {}
	
	for type in get_element_types(elements):
		themes[type] = generate_theme(element_button_theme, element_colors.data[type])
	
	element_array = atomic_sorted_elements
	element_theme = themes
	
	# hell
	# for sign in .get_signal_list():
	# 	print(sign)
	for column in convert_to_grid(elements):
		for element in column:
			if element == null:
				var empty_button = Button.new()
				empty_button.flat = true
				empty_button.disabled = true
				empty_button.theme = empty_button_theme
				add_child(empty_button)
				continue
			
			create_element_button(gen_element_data(element))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
