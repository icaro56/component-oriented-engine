#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "KernelPrerequisites.h"
#include "Collider.h"

  /**
   * @brief Um colisor mesh permite que você faça a detecção de colisão entre malhas e primitivos.
  */
class KERNEL_VS_EXPORT MeshCollider : public Collider
{
public:
	enum MeshColliderDesc
	{
		MCD_Size
	};

	/**
      * @brief Construtor Default
    */
	MeshCollider();
	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	MeshCollider(const ComponentDescMap &d);
	/**
      * @brief Destrutor Default 
    */
	virtual ~MeshCollider();
	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	/**
      * @brief Define que o colisor será convexo
      * @param c booleano de ativação
     */
	void setConvex(bool c);
	/**
      * @brief Acessa se o MeshCollider é convexo
	 */
	bool isConvex();
	/**
      * @brief Atribui a escala do BoxCollider 
      * @param s Vetor da escala desejada
     */
	virtual void setScale(const Vector3& s);

private:
	
	virtual void applyDesc(int key, const std::string& value);

private:
	ComponentDescMap mMeshColliderDescMap;

	bool convex;
};


#endif // !MESHCOLLIDER_H
