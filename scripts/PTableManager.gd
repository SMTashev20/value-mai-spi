extends GridContainer

class_name PTableManager

@export var element_colors : JSON
@export var element_data : JSON
@export var element_button_prefab : PackedScene
@export var element_scene_prefab : PackedScene

class ElementData:
	var atomic_number: int = 1
	var name: String = "Hydrogen"
	var symbol: String = "H"
	var atomic_mass: float = 1.008
	var relative_pos: Vector2i = Vector2i(1, 1)
	var color: Color = Color(.8, .8, .8)
	
	var shells: Array = [1]

func alter_color_hue(theme: Theme, name: StringName, theme_type: StringName, from_color: Color):
	var color = theme.get_color(name, theme_type)
	theme.set_color(name, theme_type, from_color)

func create_element_button(data: ElementData):
	var button = element_button_prefab.instantiate() as Button
	button.text = data.symbol
	
	button.connect("pressed", 
		func on_pressed():
			var root = get_tree().root
			var current = root.get_children()[0]
			root.remove_child(root.get_children()[0])
			
			var element_scene = element_scene_prefab.instantiate()
			root.add_child(element_scene)
			
			var model = element_scene.get_node(^"SubViewportContainer/SubViewport/ElementModelView/Base")
			print(model)
			model.set("element_data", data)
			
			current.call_deferred("free")
	)
	
	# button.theme.set_color("font_color", "Button", data.color)
	
	# alter_color_hue(button.theme, "font_color", "Button", data.color)
	# alter_color_hue(button.theme, "font_disabled_color", "Button", data.color)
	# alter_color_hue(button.theme, "font_focus_color", "Button", data.color)
	# alter_color_hue(button.theme, "font_hover_color", "Button", data.color)
	# alter_color_hue(button.theme, "font_hover_pressed_color", "Button", data.color)
	# alter_color_hue(button.theme, "font_pressed_color", "Button", data.color)
	
	add_child(button)
	
func determine_max(element_data: Array) -> Vector2i:
	var max = Vector2i(0, 0)
	
	for element in element_data:
		if element.xpos > max.x: max.x = element.xpos
		if element.ypos > max.y: max.y = element.ypos
		
	return max

func _ready():
	element_data = element_data
	var elements = element_data.data.elements
	var grid_max = determine_max(elements)
	var element_data_grid = []
	
	element_data_grid.resize(grid_max.y)
	for col_idx in element_data_grid.size():
		element_data_grid[col_idx] = []
		element_data_grid[col_idx].resize(grid_max.x)
	
	for element in elements:
		element_data_grid[element.ypos - 1][element.xpos - 1] = element
	
	var empty_button_theme = Theme.new()
	empty_button_theme.set_color("font_outline_color", "Button", "00000000")
	
	var position_counter = 1
	for col in element_data_grid:
		for element in col:
			if element == null:
				var empty_button = Button.new()
				empty_button.flat = true
				empty_button.disabled = true
				empty_button.theme = empty_button_theme
				add_child(empty_button)
				continue
			
			var data = ElementData.new()
			data.atomic_number = position_counter
			data.name = element.name
			data.symbol = element.symbol
			data.shells = element.shells
			data.atomic_mass = element.atomic_mass
			data.color = Color.from_string(element_colors.data[element.category], Color(.66, .66, .66, 1.0))
			data.relative_pos = Vector2i(element.xpos, element.ypos)
			
			create_element_button(data)
			
			position_counter += 1

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
