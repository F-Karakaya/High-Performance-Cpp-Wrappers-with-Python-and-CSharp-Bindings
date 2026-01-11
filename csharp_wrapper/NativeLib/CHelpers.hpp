#pragma once

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __attribute__((visibility("default")))
#endif

extern "C" {
typedef void *TSP_Handle;

EXPORT_API TSP_Handle TSP_Create();
EXPORT_API void TSP_Destroy(TSP_Handle handle);
EXPORT_API void TSP_LoadData(TSP_Handle handle, const double *data, int size);

// Returns a pointer to a double array, size is output via outSize.
// The caller is responsible for freeing this memory if it's a copy,
// but here we might return a copy that needs a special free function.
// To keep it simple and safe for P/Invoke, we will ask the caller to allocate
// or use a callback, but for this simpler robust wrapper, we'll return a
// pointer that must be freed by TSP_FreeDoubleArray.
EXPORT_API double *TSP_CalculateMovingAverage(TSP_Handle handle,
                                              int window_size, int *out_size);

EXPORT_API double TSP_CalculateVolatility(TSP_Handle handle);

EXPORT_API size_t *TSP_FindAnomalies(TSP_Handle handle,
                                     double z_score_threshold, int *out_size);

EXPORT_API void TSP_FreeDoubleArray(double *ptr);
EXPORT_API void TSP_FreeSizeTArray(size_t *ptr);

EXPORT_API int TSP_GetSize(TSP_Handle handle);
}
