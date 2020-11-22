; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@size = dso_local local_unnamed_addr constant i32 64, align 4
@.str = private unnamed_addr constant [18 x i8] c"\0A\0Aresult = [%d]\0A\0A\00", align 1

; Function Attrs: norecurse nounwind readonly uwtable
define dso_local i32 @foo(i32* nocapture readonly %A) local_unnamed_addr #0 !dbg !6 {
entry:
  %0 = bitcast i32* %A to <4 x i32>*, !dbg !8
  %wide.load = load <4 x i32>, <4 x i32>* %0, align 4, !dbg !8, !tbaa !9
  %1 = getelementptr inbounds i32, i32* %A, i64 4, !dbg !8
  %2 = bitcast i32* %1 to <4 x i32>*, !dbg !8
  %wide.load.1 = load <4 x i32>, <4 x i32>* %2, align 4, !dbg !8, !tbaa !9
  %3 = add <4 x i32> %wide.load.1, %wide.load, !dbg !13
  %4 = getelementptr inbounds i32, i32* %A, i64 8, !dbg !8
  %5 = bitcast i32* %4 to <4 x i32>*, !dbg !8
  %wide.load.2 = load <4 x i32>, <4 x i32>* %5, align 4, !dbg !8, !tbaa !9
  %6 = add <4 x i32> %wide.load.2, %3, !dbg !13
  %7 = getelementptr inbounds i32, i32* %A, i64 12, !dbg !8
  %8 = bitcast i32* %7 to <4 x i32>*, !dbg !8
  %wide.load.3 = load <4 x i32>, <4 x i32>* %8, align 4, !dbg !8, !tbaa !9
  %9 = add <4 x i32> %wide.load.3, %6, !dbg !13
  %10 = getelementptr inbounds i32, i32* %A, i64 16, !dbg !8
  %11 = bitcast i32* %10 to <4 x i32>*, !dbg !8
  %wide.load.4 = load <4 x i32>, <4 x i32>* %11, align 4, !dbg !8, !tbaa !9
  %12 = add <4 x i32> %wide.load.4, %9, !dbg !13
  %13 = getelementptr inbounds i32, i32* %A, i64 20, !dbg !8
  %14 = bitcast i32* %13 to <4 x i32>*, !dbg !8
  %wide.load.5 = load <4 x i32>, <4 x i32>* %14, align 4, !dbg !8, !tbaa !9
  %15 = add <4 x i32> %wide.load.5, %12, !dbg !13
  %16 = getelementptr inbounds i32, i32* %A, i64 24, !dbg !8
  %17 = bitcast i32* %16 to <4 x i32>*, !dbg !8
  %wide.load.6 = load <4 x i32>, <4 x i32>* %17, align 4, !dbg !8, !tbaa !9
  %18 = add <4 x i32> %wide.load.6, %15, !dbg !13
  %19 = getelementptr inbounds i32, i32* %A, i64 28, !dbg !8
  %20 = bitcast i32* %19 to <4 x i32>*, !dbg !8
  %wide.load.7 = load <4 x i32>, <4 x i32>* %20, align 4, !dbg !8, !tbaa !9
  %21 = add <4 x i32> %wide.load.7, %18, !dbg !13
  %22 = getelementptr inbounds i32, i32* %A, i64 32, !dbg !8
  %23 = bitcast i32* %22 to <4 x i32>*, !dbg !8
  %wide.load.8 = load <4 x i32>, <4 x i32>* %23, align 4, !dbg !8, !tbaa !9
  %24 = add <4 x i32> %wide.load.8, %21, !dbg !13
  %25 = getelementptr inbounds i32, i32* %A, i64 36, !dbg !8
  %26 = bitcast i32* %25 to <4 x i32>*, !dbg !8
  %wide.load.9 = load <4 x i32>, <4 x i32>* %26, align 4, !dbg !8, !tbaa !9
  %27 = add <4 x i32> %wide.load.9, %24, !dbg !13
  %28 = getelementptr inbounds i32, i32* %A, i64 40, !dbg !8
  %29 = bitcast i32* %28 to <4 x i32>*, !dbg !8
  %wide.load.10 = load <4 x i32>, <4 x i32>* %29, align 4, !dbg !8, !tbaa !9
  %30 = add <4 x i32> %wide.load.10, %27, !dbg !13
  %31 = getelementptr inbounds i32, i32* %A, i64 44, !dbg !8
  %32 = bitcast i32* %31 to <4 x i32>*, !dbg !8
  %wide.load.11 = load <4 x i32>, <4 x i32>* %32, align 4, !dbg !8, !tbaa !9
  %33 = add <4 x i32> %wide.load.11, %30, !dbg !13
  %34 = getelementptr inbounds i32, i32* %A, i64 48, !dbg !8
  %35 = bitcast i32* %34 to <4 x i32>*, !dbg !8
  %wide.load.12 = load <4 x i32>, <4 x i32>* %35, align 4, !dbg !8, !tbaa !9
  %36 = add <4 x i32> %wide.load.12, %33, !dbg !13
  %37 = getelementptr inbounds i32, i32* %A, i64 52, !dbg !8
  %38 = bitcast i32* %37 to <4 x i32>*, !dbg !8
  %wide.load.13 = load <4 x i32>, <4 x i32>* %38, align 4, !dbg !8, !tbaa !9
  %39 = add <4 x i32> %wide.load.13, %36, !dbg !13
  %40 = getelementptr inbounds i32, i32* %A, i64 56, !dbg !8
  %41 = bitcast i32* %40 to <4 x i32>*, !dbg !8
  %wide.load.14 = load <4 x i32>, <4 x i32>* %41, align 4, !dbg !8, !tbaa !9
  %42 = add <4 x i32> %wide.load.14, %39, !dbg !13
  %43 = getelementptr inbounds i32, i32* %A, i64 60, !dbg !8
  %44 = bitcast i32* %43 to <4 x i32>*, !dbg !8
  %wide.load.15 = load <4 x i32>, <4 x i32>* %44, align 4, !dbg !8, !tbaa !9
  %45 = add <4 x i32> %wide.load.15, %42, !dbg !13
  %rdx.shuf = shufflevector <4 x i32> %45, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>, !dbg !14
  %bin.rdx = add <4 x i32> %45, %rdx.shuf, !dbg !14
  %rdx.shuf8 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>, !dbg !14
  %bin.rdx9 = add <4 x i32> %bin.rdx, %rdx.shuf8, !dbg !14
  %46 = extractelement <4 x i32> %bin.rdx9, i32 0, !dbg !14
  ret i32 %46, !dbg !15
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #2 !dbg !16 {
entry:
  %A = alloca [64 x i32], align 16
  %0 = bitcast [64 x i32]* %A to i8*, !dbg !17
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %0) #4, !dbg !17
  %1 = bitcast [64 x i32]* %A to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %1, align 16, !dbg !18, !tbaa !9
  %2 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 4, !dbg !19
  %3 = bitcast i32* %2 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %3, align 16, !dbg !18, !tbaa !9
  %4 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 8, !dbg !19
  %5 = bitcast i32* %4 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %5, align 16, !dbg !18, !tbaa !9
  %6 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 12, !dbg !19
  %7 = bitcast i32* %6 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %7, align 16, !dbg !18, !tbaa !9
  %8 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 16, !dbg !19
  %9 = bitcast i32* %8 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %9, align 16, !dbg !18, !tbaa !9
  %10 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 20, !dbg !19
  %11 = bitcast i32* %10 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %11, align 16, !dbg !18, !tbaa !9
  %12 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 24, !dbg !19
  %13 = bitcast i32* %12 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %13, align 16, !dbg !18, !tbaa !9
  %14 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 28, !dbg !19
  %15 = bitcast i32* %14 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %15, align 16, !dbg !18, !tbaa !9
  %16 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 32, !dbg !19
  %17 = bitcast i32* %16 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %17, align 16, !dbg !18, !tbaa !9
  %18 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 36, !dbg !19
  %19 = bitcast i32* %18 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %19, align 16, !dbg !18, !tbaa !9
  %20 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 40, !dbg !19
  %21 = bitcast i32* %20 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %21, align 16, !dbg !18, !tbaa !9
  %22 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 44, !dbg !19
  %23 = bitcast i32* %22 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %23, align 16, !dbg !18, !tbaa !9
  %24 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 48, !dbg !19
  %25 = bitcast i32* %24 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %25, align 16, !dbg !18, !tbaa !9
  %26 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 52, !dbg !19
  %27 = bitcast i32* %26 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %27, align 16, !dbg !18, !tbaa !9
  %28 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 56, !dbg !19
  %29 = bitcast i32* %28 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %29, align 16, !dbg !18, !tbaa !9
  %30 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 60, !dbg !19
  %31 = bitcast i32* %30 to <4 x i32>*, !dbg !18
  store <4 x i32> <i32 1, i32 1, i32 1, i32 1>, <4 x i32>* %31, align 16, !dbg !18, !tbaa !9
  %32 = bitcast [64 x i32]* %A to <4 x i32>*, !dbg !20
  %wide.load = load <4 x i32>, <4 x i32>* %32, align 16, !dbg !20, !tbaa !9
  %33 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 4, !dbg !20
  %34 = bitcast i32* %33 to <4 x i32>*, !dbg !20
  %wide.load.1 = load <4 x i32>, <4 x i32>* %34, align 16, !dbg !20, !tbaa !9
  %35 = add <4 x i32> %wide.load.1, %wide.load, !dbg !22
  %36 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 8, !dbg !20
  %37 = bitcast i32* %36 to <4 x i32>*, !dbg !20
  %wide.load.2 = load <4 x i32>, <4 x i32>* %37, align 16, !dbg !20, !tbaa !9
  %38 = add <4 x i32> %wide.load.2, %35, !dbg !22
  %39 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 12, !dbg !20
  %40 = bitcast i32* %39 to <4 x i32>*, !dbg !20
  %wide.load.3 = load <4 x i32>, <4 x i32>* %40, align 16, !dbg !20, !tbaa !9
  %41 = add <4 x i32> %wide.load.3, %38, !dbg !22
  %42 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 16, !dbg !20
  %43 = bitcast i32* %42 to <4 x i32>*, !dbg !20
  %wide.load.4 = load <4 x i32>, <4 x i32>* %43, align 16, !dbg !20, !tbaa !9
  %44 = add <4 x i32> %wide.load.4, %41, !dbg !22
  %45 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 20, !dbg !20
  %46 = bitcast i32* %45 to <4 x i32>*, !dbg !20
  %wide.load.5 = load <4 x i32>, <4 x i32>* %46, align 16, !dbg !20, !tbaa !9
  %47 = add <4 x i32> %wide.load.5, %44, !dbg !22
  %48 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 24, !dbg !20
  %49 = bitcast i32* %48 to <4 x i32>*, !dbg !20
  %wide.load.6 = load <4 x i32>, <4 x i32>* %49, align 16, !dbg !20, !tbaa !9
  %50 = add <4 x i32> %wide.load.6, %47, !dbg !22
  %51 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 28, !dbg !20
  %52 = bitcast i32* %51 to <4 x i32>*, !dbg !20
  %wide.load.7 = load <4 x i32>, <4 x i32>* %52, align 16, !dbg !20, !tbaa !9
  %53 = add <4 x i32> %wide.load.7, %50, !dbg !22
  %54 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 32, !dbg !20
  %55 = bitcast i32* %54 to <4 x i32>*, !dbg !20
  %wide.load.8 = load <4 x i32>, <4 x i32>* %55, align 16, !dbg !20, !tbaa !9
  %56 = add <4 x i32> %wide.load.8, %53, !dbg !22
  %57 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 36, !dbg !20
  %58 = bitcast i32* %57 to <4 x i32>*, !dbg !20
  %wide.load.9 = load <4 x i32>, <4 x i32>* %58, align 16, !dbg !20, !tbaa !9
  %59 = add <4 x i32> %wide.load.9, %56, !dbg !22
  %60 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 40, !dbg !20
  %61 = bitcast i32* %60 to <4 x i32>*, !dbg !20
  %wide.load.10 = load <4 x i32>, <4 x i32>* %61, align 16, !dbg !20, !tbaa !9
  %62 = add <4 x i32> %wide.load.10, %59, !dbg !22
  %63 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 44, !dbg !20
  %64 = bitcast i32* %63 to <4 x i32>*, !dbg !20
  %wide.load.11 = load <4 x i32>, <4 x i32>* %64, align 16, !dbg !20, !tbaa !9
  %65 = add <4 x i32> %wide.load.11, %62, !dbg !22
  %66 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 48, !dbg !20
  %67 = bitcast i32* %66 to <4 x i32>*, !dbg !20
  %wide.load.12 = load <4 x i32>, <4 x i32>* %67, align 16, !dbg !20, !tbaa !9
  %68 = add <4 x i32> %wide.load.12, %65, !dbg !22
  %69 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 52, !dbg !20
  %70 = bitcast i32* %69 to <4 x i32>*, !dbg !20
  %wide.load.13 = load <4 x i32>, <4 x i32>* %70, align 16, !dbg !20, !tbaa !9
  %71 = add <4 x i32> %wide.load.13, %68, !dbg !22
  %72 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 56, !dbg !20
  %73 = bitcast i32* %72 to <4 x i32>*, !dbg !20
  %wide.load.14 = load <4 x i32>, <4 x i32>* %73, align 16, !dbg !20, !tbaa !9
  %74 = add <4 x i32> %wide.load.14, %71, !dbg !22
  %75 = getelementptr inbounds [64 x i32], [64 x i32]* %A, i64 0, i64 60, !dbg !20
  %76 = bitcast i32* %75 to <4 x i32>*, !dbg !20
  %wide.load.15 = load <4 x i32>, <4 x i32>* %76, align 16, !dbg !20, !tbaa !9
  %77 = add <4 x i32> %wide.load.15, %74, !dbg !22
  %rdx.shuf = shufflevector <4 x i32> %77, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>, !dbg !23
  %bin.rdx = add <4 x i32> %77, %rdx.shuf, !dbg !23
  %rdx.shuf18 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>, !dbg !23
  %bin.rdx19 = add <4 x i32> %bin.rdx, %rdx.shuf18, !dbg !23
  %78 = extractelement <4 x i32> %bin.rdx19, i32 0, !dbg !23
  %call1 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i64 0, i64 0), i32 %78), !dbg !24
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %0) #4, !dbg !25
  ret i32 0, !dbg !26
}

; Function Attrs: nofree nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

attributes #0 = { norecurse nounwind readonly uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}
!llvm.ident = !{!5}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "input.c", directory: "/home/tiba/workspace/ssOptimizer/SsLoopVec/test-inputs")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
!6 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 5, type: !7, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 9, column: 10, scope: !6)
!9 = !{!10, !10, i64 0}
!10 = !{!"int", !11, i64 0}
!11 = !{!"omnipotent char", !12, i64 0}
!12 = !{!"Simple C/C++ TBAA"}
!13 = !DILocation(line: 9, column: 7, scope: !6)
!14 = !DILocation(line: 8, column: 3, scope: !6)
!15 = !DILocation(line: 11, column: 3, scope: !6)
!16 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 15, type: !7, scopeLine: 15, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!17 = !DILocation(line: 16, column: 3, scope: !16)
!18 = !DILocation(line: 18, column: 10, scope: !16)
!19 = !DILocation(line: 18, column: 5, scope: !16)
!20 = !DILocation(line: 9, column: 10, scope: !6, inlinedAt: !21)
!21 = distinct !DILocation(line: 20, column: 11, scope: !16)
!22 = !DILocation(line: 9, column: 7, scope: !6, inlinedAt: !21)
!23 = !DILocation(line: 8, column: 3, scope: !6, inlinedAt: !21)
!24 = !DILocation(line: 21, column: 3, scope: !16)
!25 = !DILocation(line: 23, column: 1, scope: !16)
!26 = !DILocation(line: 22, column: 3, scope: !16)
