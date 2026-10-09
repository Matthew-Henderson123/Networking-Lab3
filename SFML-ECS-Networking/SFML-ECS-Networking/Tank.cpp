#include "tank.h"
#include "utils.h"

Tank::Tank(std::string colour)
{
	// Save input colour in case we need it later.
	colorString = colour;

	// Load textures.
	// FIXME: loadFromFile returns a bool if texture was loaded successfully. We should use it to check for errors.
	if (!bodyTexture.loadFromFile("Assets/" + colour + "Tank.png"))
	{
		Utils::printMsg("Failed to load tank body image.", error);
	}
	if (!barrelTexture.loadFromFile("Assets/" + colour + "Barrel.png"))
	{
		Utils::printMsg("Failed to load tank barrel image.", error);
	}

	// Apply tetxures to sprites.
	body.setTexture(bodyTexture);
	barrel.setTexture(barrelTexture);

	// Reset texture rectangle. Applying new texture does not automatically apply it's size to sprite.
	body.setTextureRect(sf::IntRect({ 0, 0 }, (sf::Vector2i)bodyTexture.getSize()));
	barrel.setTextureRect(sf::IntRect({ 0, 0 }, (sf::Vector2i)barrelTexture.getSize()));

	// Set sprite origins. For bodym use the center of the texture. For barrel, hardcoded value.
	body.setOrigin((sf::Vector2f)body.getTextureRect().getCenter());
	barrel.setOrigin({ 6, 2 });

	// With the correct offset on the barrel, we can just set barrel position = body position.
	body.setPosition(position);
	barrel.setPosition(body.getPosition());

	// Set default barrel rotation to match body rotation.
	// FIXME: for actual tank game, we would have barrel rotated independently of the body.
	body.setRotation(bodyRotation);
	barrel.setRotation(barrelRotation);
}

void Tank::Update(float dt)
{
	// Update rotation angle based on input.
	if (isMoving.left)
		bodyRotation -= sf::degrees(rotationSpeed * dt);
	else if (isMoving.right)
		bodyRotation += sf::degrees(rotationSpeed * dt);

	if (isMoving.t_left)
		barrelRotation -= sf::degrees(rotationSpeed * dt);
	else if (isMoving.t_right)
		barrelRotation += sf::degrees(rotationSpeed * dt);

	if (barrelRotation.asDegrees() > 360)
		barrelRotation = sf::degrees(0);
	else if (barrelRotation.asDegrees() < 0)
		barrelRotation = sf::degrees(360);

	if (bodyRotation.asDegrees() > 360)
		bodyRotation = sf::degrees(0);
	else if (bodyRotation.asDegrees() < 0)
		bodyRotation = sf::degrees(360);

	// Calculate direction vector from angle of rotation.
	sf::Vector2f body_direction = {
		std::cos((bodyRotation - sf::degrees(90)).asRadians()),
		std::sin((bodyRotation - sf::degrees(90)).asRadians())
	};

	sf::Vector2f barrel_direction = {
		std::cos((barrelRotation - sf::degrees(90)).asRadians()),
		std::sin((barrelRotation - sf::degrees(90)).asRadians())
	};

	// Update position based on input and direction.
	if (isMoving.forward)
		position -= body_direction * movementSpeed * dt;
	else if (isMoving.backward)
		position += body_direction * movementSpeed * dt;

	// Apply new rotation to tank body and barrel.
	body.setRotation(bodyRotation);
	barrel.setRotation(barrelRotation);

	// Apply new position to tank body and barrel.
	body.setPosition(position);
	barrel.setPosition(position);
}

const void Tank::Render(sf::RenderWindow &window) {
		window.draw(body);
		window.draw(barrel);
}

void Tank::SetColourString(std::string col)
{
	colorString = col;
}

std::string Tank::GetColourString()
{
	return colorString;
}
