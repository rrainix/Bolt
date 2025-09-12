# Install script for directory: C:/Users/benso/source/repos/Bolt/BoltCore

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Bolt")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/benso/source/repos/Bolt/BoltCore/Debug/BoltCore.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/benso/source/repos/Bolt/BoltCore/Release/BoltCore.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/benso/source/repos/Bolt/BoltCore/MinSizeRel/BoltCore.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/benso/source/repos/Bolt/BoltCore/RelWithDebInfo/BoltCore.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Bolt.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/EntryPoint.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/btpch.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Audio" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/Audio.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioManager.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioSource.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Collections" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/AABB.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Color.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Mat2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Quaternion.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec4.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Viewport.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Components" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/BoxCollider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Collider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ComponentUtils.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ParticleSystem2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Rigidbody2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/SpriteRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Tags.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Transform2D.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Core" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Application.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/GLFWWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Input.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/KeyCodes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/SokolWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Time.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Debug" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Debug/Logger.hpp")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Graphics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Camera2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/GizmoRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Gizmos.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/QuadMesh.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderContext.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderResources.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Renderer2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Shader.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Texture2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Math" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Common.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Math.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Trigonometry.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/VectorMath.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Physics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Box2DWorld.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Collision2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/CollisionDispatcher.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Physics2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsSystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsTypes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsUtility.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Scene" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Entity.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/EntityHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/ISystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Scene.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneDefinition.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Utils" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Async.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Random.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Serializer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Timer.hpp"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Bolt.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/EntryPoint.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/btpch.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Audio" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/Audio.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioManager.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioSource.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Collections" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/AABB.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Color.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Mat2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Quaternion.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec4.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Viewport.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Components" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/BoxCollider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Collider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ComponentUtils.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ParticleSystem2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Rigidbody2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/SpriteRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Tags.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Transform2D.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Core" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Application.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/GLFWWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Input.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/KeyCodes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/SokolWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Time.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Debug" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Debug/Logger.hpp")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Graphics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Camera2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/GizmoRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Gizmos.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/QuadMesh.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderContext.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderResources.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Renderer2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Shader.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Texture2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Math" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Common.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Math.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Trigonometry.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/VectorMath.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Physics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Box2DWorld.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Collision2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/CollisionDispatcher.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Physics2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsSystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsTypes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsUtility.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Scene" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Entity.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/EntityHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/ISystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Scene.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneDefinition.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Utils" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Async.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Random.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Serializer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Timer.hpp"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Bolt.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/EntryPoint.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/btpch.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Audio" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/Audio.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioManager.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioSource.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Collections" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/AABB.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Color.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Mat2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Quaternion.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec4.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Viewport.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Components" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/BoxCollider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Collider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ComponentUtils.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ParticleSystem2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Rigidbody2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/SpriteRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Tags.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Transform2D.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Core" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Application.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/GLFWWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Input.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/KeyCodes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/SokolWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Time.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Debug" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Debug/Logger.hpp")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Graphics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Camera2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/GizmoRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Gizmos.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/QuadMesh.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderContext.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderResources.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Renderer2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Shader.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Texture2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Math" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Common.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Math.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Trigonometry.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/VectorMath.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Physics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Box2DWorld.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Collision2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/CollisionDispatcher.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Physics2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsSystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsTypes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsUtility.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Scene" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Entity.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/EntityHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/ISystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Scene.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneDefinition.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Utils" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Async.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Random.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Serializer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Timer.hpp"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Bolt.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/EntryPoint.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/btpch.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Audio" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/Audio.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioManager.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Audio/AudioSource.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Collections" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/AABB.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Color.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Mat2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Quaternion.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec2.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Vec4.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Collections/Viewport.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Components" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/BoxCollider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Collider2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ComponentUtils.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/ParticleSystem2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Rigidbody2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/SpriteRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Tags.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Components/Transform2D.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Core" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Application.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/GLFWWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Input.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/KeyCodes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/SokolWindow.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Core/Time.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Debug" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Debug/Logger.hpp")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Graphics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Camera2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/GizmoRenderer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Gizmos.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/QuadMesh.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderContext.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/RenderResources.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Renderer2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Shader.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/Texture2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Graphics/TextureManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Math" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Common.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Math.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/Trigonometry.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Math/VectorMath.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Physics" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Box2DWorld.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Collision2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/CollisionDispatcher.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/Physics2D.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsSystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsTypes.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Physics/PhysicsUtility.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Scene" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Entity.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/EntityHandle.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/ISystem.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/Scene.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneDefinition.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Scene/SceneManager.hpp"
      )
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Bolt/Utils" TYPE FILE FILES
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Async.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Random.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Serializer.hpp"
      "C:/Users/benso/source/repos/Bolt/BoltCore/include/Bolt/Utils/Timer.hpp"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore/BoltCoreConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore/BoltCoreConfig.cmake"
         "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore/BoltCoreConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore/BoltCoreConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/BoltCore" TYPE FILE FILES "C:/Users/benso/source/repos/Bolt/BoltCore/CMakeFiles/Export/6697f1ec717cc04488da6ccf8356b7c1/BoltCoreConfig-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/benso/source/repos/Bolt/BoltCore/include/")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/benso/source/repos/Bolt/BoltCore/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
