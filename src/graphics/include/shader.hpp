#pragma once

#include <GL/gl.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

inline std::string readShaderFile(const std::string &path)
{
    std::ifstream file_stream(path); // 주어진 경로의 파일을 연다
    if (!file_stream.is_open())      // 파일 열기 실패 확인
    {
        throw std::runtime_error("Failed to open shader: " + path); // 실패 시 예외 발생
    }
    std::stringstream buffer;      // 전체 내용을 담을 버퍼
    buffer << file_stream.rdbuf(); // 파일 스트림 내용을 모두 버퍼에 쓴다
    return buffer.str();           // 문자열로 반환
}

inline GLuint compileShader(GLenum type, const std::string &source)
{
    GLuint shader = glCreateShader(type);                // 셰이더 객체 생성
    const char *src = source.c_str();                    // GLSL 문자열 포인터 얻기
    glShaderSource(shader, 1, &src, nullptr);            // 셰이더에 소스 코드 등록
    glCompileShader(shader);                             // GLSL 컴파일 실행
    GLint compiled = 0;                                  // 컴파일 성공 여부 저장용 변수
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); // 컴파일 성공 여부 확인
    if (compiled != GL_TRUE)                             // 실패 시
    {
        char log[1024];                                                         // 에러 로그 버퍼
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);                  // GLSL 컴파일 로그 읽기
        throw std::runtime_error(std::string("Shader compile failed: ") + log); // 예외로 에러 전달
    }
    return shader; // 성공 시 셰이더 ID 반환
}