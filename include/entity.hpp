/*
 * include/entity.hpp
 *
 * This is the basic entity, this gets extended later on in source
 * and abstracted away to things like the player or non-player entities.
 * This is technically an interface, like Java. All the provided funcs
 * are virtual; meaning you need to override them in subclasses.
 * 
 * Created: 15 April 2026
 */
 
class IBaseEntity
{
public:
	virtual ~IBaseEntity() = default;
	
	/* Per tick updates should be happening in here! */
	virtual void Update() = 0;
	
	/* This handles render calls to Direct8 */
	virtual void Render() = 0;
	
	/* This just gives us the assetPath of the entity, or nothing. */
	const char* GetAssetPath() const { return m_assetPath; }

protected:
	const char* m_assetPath;
};