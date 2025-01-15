option(USE_STATIC_MBEDTLS_LIBRARY "Build mbed TLS static library." ON)
option(USE_SHARED_MBEDTLS_LIBRARY "Build mbed TLS shared library." OFF)
option(LINK_WITH_PTHREAD "Explicitly link mbed TLS library to pthread." OFF)

add_compile_definitions(MBEDTLS_CONFIG_FILE=<mbedtls_config.h>)

set(src_crypto
    ../../Middlewares/Third_Party/mbedTLS/library/aes.c
    ../../Middlewares/Third_Party/mbedTLS/library/aesni.c
    ../../Middlewares/Third_Party/mbedTLS/library/arc4.c
    ../../Middlewares/Third_Party/mbedTLS/library/aria.c
    ../../Middlewares/Third_Party/mbedTLS/library/asn1parse.c
    ../../Middlewares/Third_Party/mbedTLS/library/asn1write.c
    ../../Middlewares/Third_Party/mbedTLS/library/base64.c
    ../../Middlewares/Third_Party/mbedTLS/library/bignum.c
    ../../Middlewares/Third_Party/mbedTLS/library/blowfish.c
    ../../Middlewares/Third_Party/mbedTLS/library/camellia.c
    ../../Middlewares/Third_Party/mbedTLS/library/ccm.c
    ../../Middlewares/Third_Party/mbedTLS/library/chacha20.c
    ../../Middlewares/Third_Party/mbedTLS/library/chachapoly.c
    ../../Middlewares/Third_Party/mbedTLS/library/cipher.c
    ../../Middlewares/Third_Party/mbedTLS/library/cipher_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/cmac.c
    ../../Middlewares/Third_Party/mbedTLS/library/ctr_drbg.c
    ../../Middlewares/Third_Party/mbedTLS/library/des.c
    ../../Middlewares/Third_Party/mbedTLS/library/dhm.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecdh.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecdsa.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecjpake.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecp.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecp_curves.c
    ../../Middlewares/Third_Party/mbedTLS/library/entropy.c
    ../../Middlewares/Third_Party/mbedTLS/library/entropy_poll.c
    ../../Middlewares/Third_Party/mbedTLS/library/error.c
    ../../Middlewares/Third_Party/mbedTLS/library/gcm.c
    ../../Middlewares/Third_Party/mbedTLS/library/havege.c
    ../../Middlewares/Third_Party/mbedTLS/library/hkdf.c
    ../../Middlewares/Third_Party/mbedTLS/library/hmac_drbg.c
    ../../Middlewares/Third_Party/mbedTLS/library/md.c
    ../../Middlewares/Third_Party/mbedTLS/library/md2.c
    ../../Middlewares/Third_Party/mbedTLS/library/md4.c
    ../../Middlewares/Third_Party/mbedTLS/library/md5.c
    ../../Middlewares/Third_Party/mbedTLS/library/md_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/memory_buffer_alloc.c
    ../../Middlewares/Third_Party/mbedTLS/library/nist_kw.c
    ../../Middlewares/Third_Party/mbedTLS/library/oid.c
    ../../Middlewares/Third_Party/mbedTLS/library/padlock.c
    ../../Middlewares/Third_Party/mbedTLS/library/pem.c
    ../../Middlewares/Third_Party/mbedTLS/library/pk.c
    ../../Middlewares/Third_Party/mbedTLS/library/pk_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkcs12.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkcs5.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkparse.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkwrite.c
    ../../Middlewares/Third_Party/mbedTLS/library/platform.c
    ../../Middlewares/Third_Party/mbedTLS/library/platform_util.c
    ../../Middlewares/Third_Party/mbedTLS/library/poly1305.c
    ../../Middlewares/Third_Party/mbedTLS/library/ripemd160.c
    ../../Middlewares/Third_Party/mbedTLS/library/rsa.c
    ../../Middlewares/Third_Party/mbedTLS/library/rsa_internal.c
    ../../Middlewares/Third_Party/mbedTLS/library/sha1.c
    ../../Middlewares/Third_Party/mbedTLS/library/sha256.c
    ../../Middlewares/Third_Party/mbedTLS/library/sha512.c
    ../../Middlewares/Third_Party/mbedTLS/library/threading.c
    ../../Middlewares/Third_Party/mbedTLS/library/timing.c
    ../../Middlewares/Third_Party/mbedTLS/library/version.c
    ../../Middlewares/Third_Party/mbedTLS/library/version_features.c
    ../../Middlewares/Third_Party/mbedTLS/library/xtea.c
)

set(src_x509
    ../../Middlewares/Third_Party/mbedTLS/library/certs.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkcs11.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509_create.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509_crl.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509_crt.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509_csr.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509write_crt.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509write_csr.c
)

set(src_tls
    ../../Middlewares/Third_Party/mbedTLS/library/debug.c
    #../../Middlewares/Third_Party/mbedTLS/library/net_sockets.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_cache.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_ciphersuites.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_cli.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_cookie.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_srv.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_ticket.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_tls.c
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-declarations -Wmissing-prototypes")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-declarations -Wmissing-prototypes -Wdocumentation -Wno-documentation-deprecated-sync -Wunreachable-code")
endif(CMAKE_COMPILER_IS_CLANG)

if(UNSAFE_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error")
    set(CMAKE_C_FLAGS_ASAN "${CMAKE_C_FLAGS_ASAN} -Wno-error")
    set(CMAKE_C_FLAGS_ASANDBG "${CMAKE_C_FLAGS_ASANDBG} -Wno-error")
endif(UNSAFE_BUILD)

if(WIN32)
    set(libs ${libs} ws2_32)
endif(WIN32)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
    SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

if(HAIKU)
    set(libs ${libs} network)
endif(HAIKU)

if(USE_PKCS11_HELPER_LIBRARY)
    set(libs ${libs} pkcs11-helper)
endif(USE_PKCS11_HELPER_LIBRARY)

if(ENABLE_ZLIB_SUPPORT)
    set(libs ${libs} ${ZLIB_LIBRARIES})
endif(ENABLE_ZLIB_SUPPORT)

if(LINK_WITH_PTHREAD)
    set(libs ${libs} pthread)
endif()

if (NOT USE_STATIC_MBEDTLS_LIBRARY AND NOT USE_SHARED_MBEDTLS_LIBRARY)
    message(FATAL_ERROR "Need to choose static or shared mbedtls build!")
endif(NOT USE_STATIC_MBEDTLS_LIBRARY AND NOT USE_SHARED_MBEDTLS_LIBRARY)

if(USE_STATIC_MBEDTLS_LIBRARY AND USE_SHARED_MBEDTLS_LIBRARY)
    set(mbedtls_static_target "mbedtls_static")
    set(mbedx509_static_target "mbedx509_static")
    set(mbedcrypto_static_target "mbedcrypto_static")
elseif(USE_STATIC_MBEDTLS_LIBRARY)
    set(mbedtls_static_target "mbedtls")
    set(mbedx509_static_target "mbedx509")
    set(mbedcrypto_static_target "mbedcrypto")
endif()


if(USE_STATIC_MBEDTLS_LIBRARY)
    add_library(${mbedcrypto_static_target} STATIC ${src_crypto})
    set_target_properties(${mbedcrypto_static_target} PROPERTIES OUTPUT_NAME mbedcrypto)
    target_link_libraries(${mbedcrypto_static_target} ${libs})
    target_include_directories(${mbedcrypto_static_target} 
        PUBLIC 
        ../../MBEDTLS/App
        ../../Middlewares/Third_Party/mbedTLS/include
        )

    add_library(${mbedx509_static_target} STATIC ${src_x509})
    set_target_properties(${mbedx509_static_target} PROPERTIES OUTPUT_NAME mbedx509)
    target_link_libraries(${mbedx509_static_target} ${libs} ${mbedcrypto_static_target})

    add_library(${mbedtls_static_target} STATIC ${src_tls})
    set_target_properties(${mbedtls_static_target} PROPERTIES OUTPUT_NAME mbedtls)
    target_link_libraries(${mbedtls_static_target} ${libs} ${mbedx509_static_target})

    install(TARGETS ${mbedtls_static_target} ${mbedx509_static_target} ${mbedcrypto_static_target}
            DESTINATION ${LIB_INSTALL_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(USE_STATIC_MBEDTLS_LIBRARY)

if(USE_SHARED_MBEDTLS_LIBRARY)
    add_library(mbedcrypto SHARED ${src_crypto})
    set_target_properties(mbedcrypto PROPERTIES VERSION 2.16.2 SOVERSION 3)
    target_link_libraries(mbedcrypto ${libs})

    add_library(mbedx509 SHARED ${src_x509})
    set_target_properties(mbedx509 PROPERTIES VERSION 2.16.2 SOVERSION 0)
    target_link_libraries(mbedx509 ${libs} mbedcrypto)

    add_library(mbedtls SHARED ${src_tls})
    set_target_properties(mbedtls PROPERTIES VERSION 2.16.2 SOVERSION 12)
    target_link_libraries(mbedtls ${libs} mbedx509)

    install(TARGETS mbedtls mbedx509 mbedcrypto
            DESTINATION ${LIB_INSTALL_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(USE_SHARED_MBEDTLS_LIBRARY)

add_custom_target(lib DEPENDS mbedcrypto mbedx509 mbedtls)
if(USE_STATIC_MBEDTLS_LIBRARY AND USE_SHARED_MBEDTLS_LIBRARY)
    add_dependencies(lib mbedcrypto_static mbedx509_static mbedtls_static)
endif()
