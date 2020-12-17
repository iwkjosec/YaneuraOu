#ifndef __NN_GPU_H_INCLUDED__
#define __NN_GPU_H_INCLUDED__
#include "../../config.h"

#if defined(YANEURAOU_ENGINE_DEEP)

#if defined(ONNXRUNTIME)

	// ONNX Runtimeを使う場合。
	// "docs/解説.txt"を確認すること。

	#include <onnxruntime_cxx_api.h>
#endif

#include "nn_evaluate.h"

namespace Eval::dlshogi
{
	// ニューラルネットの基底クラス
	class NN
	{
	public:
		// forwardに渡すメモリの確保
		// size個分だけDTypeを確保して返してくれる。
		// TODO : 派生クラス側でoverrideするかも。
		void* alloc(size_t size);
		
		// alloc()で確保したメモリの開放
		// TODO : 派生クラス側でoverrideするかも。
		void free(void* ptr);

		// NNによる推論。
		virtual void forward(const int batch_size, NN_Input1* x1, NN_Input2* x2, NN_Output_Policy* y1, NN_Output_Value* y2) = 0;

		// モデルファイルの読み込み。
		virtual Tools::Result load(const std::string& model_path , int gpu_id) = 0;

		// モデルファイル名を渡すとそれに応じたNN派生クラスをbuildして返してくれる。デザパタで言うところのbuilder。
		static std::shared_ptr<NN> build_nn(const std::string& model_path , int gpu_id);

		// 派生クラス側のデストラクタ呼び出されてほしいのでこれ用意しとく。
		virtual ~NN() {}
	};

#if defined(ONNXRUNTIME)

	// ONNXRUNTIME用
	class NNOnnxRuntime : public NN
	{
	public:
		// モデルファイルの読み込み。
		virtual Tools::Result load(const std::string& model_path , int gpu_id);

		// NNによる推論
		virtual void forward(const int batch_size, NN_Input1* x1, NN_Input2* x2, NN_Output_Policy* y1, NN_Output_Value* y2);

	private:
		Ort::Env env;
		std::unique_ptr<Ort::Session> session;
		Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

	};
#endif

} // namespace Eval::dlshogi

#endif // defined(YANEURAOU_ENGINE_DEEP)
#endif // ndef __NN_GPU_H_INCLUDED__
